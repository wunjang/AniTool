#ifndef ENGINE_FUNCTION_H__
#define ENGINE_FUNCTION_H__

namespace ENGINE
{
	template <typename T>
	DWORD Safe_AddRef(T& pInstance)
	{
		DWORD	dwRefCnt = 0;
		if (nullptr != pInstance)
			dwRefCnt = pInstance->AddRef();
		return dwRefCnt;
	}


	template <typename T>
	DWORD Safe_Release(T& pointer)
	{
		DWORD	dwRefCnt = 0;
		if (NULL != pointer)
		{

			dwRefCnt = pointer->Release();
			if (dwRefCnt > 1000)
			{
				int i = 0;
			}
			if (dwRefCnt == 0)
				pointer = NULL;
		}
		return dwRefCnt;
	}

	template <typename T>
	void Safe_Single_Destory(T& pointer)
	{
		if (NULL != pointer)
		{
			pointer->DestroyInstance();
			pointer = NULL;
		}
	}

	template <typename T>
	void Safe_Delete(T& pointer)
	{
		if (NULL != pointer)
		{
			delete pointer;
			pointer = NULL;
		}
	}

	template <typename T>
	void Safe_Delete_Array(T& pointer)
	{
		if (NULL != pointer)
		{
			delete[] pointer;
			pointer = NULL;
		}
	}

	//////////////////////////////////////////////////////////////////
	/////////////////////////////////Functor//////////////////////////

	class CTag_Finder
	{
	public:
		explicit CTag_Finder(const _tchar* pTag)
			: m_pTargetTag(pTag)
		{
		}
		~CTag_Finder() {		}
	public:
		template<typename T>
		bool operator()(const T& pair)
		{
			if (0 == lstrcmpW(m_pTargetTag, pair.first))
			{
				return true;
			}

			return false;
		}

	private:
		const _tchar*		m_pTargetTag = nullptr;
	};

	class CKey_Finder
	{
	public:
		explicit CKey_Finder(const wstring pTag)
			: m_pTargetKey(pTag)
		{
		}
		~CKey_Finder() {		}
	public:
		template<typename T>
		bool operator()(const T& pair)
		{
			if (m_pTargetKey == pair.first)
			{
				return true;
			}

			return false;
		}

	private:
		const wstring		m_pTargetKey = nullptr;
	};


	class CDeleteObj
	{
	public:
		explicit CDeleteObj(void) {}
		~CDeleteObj(void) {}
	public: // operator
		template <typename T>
		void operator () (T& pInstance)
		{
			_ulong dwRefCnt = 0;

			dwRefCnt = pInstance->Release();

			if (0 == dwRefCnt)
				pInstance = nullptr;
		}
	};

	// 연관컨테이너 삭제용
	class CDeleteMap
	{
	public:
		explicit CDeleteMap(void) {}
		~CDeleteMap(void) {}
	public: // operator	
		template <typename T>
		void operator () (T& Pair)
		{
			_ulong dwRefCnt = 0;


			dwRefCnt = Pair.second->Release();

			if (0 == dwRefCnt)
				Pair.second = NULL;
		}
	};


	////////////////////////충돌용/////////////////////////////////////
	class Collision_SphereToBox
	{
	public:
		explicit Collision_SphereToBox()
		{
		}
		explicit Collision_SphereToBox(const _vec3 vSpherePos, const _float fSphereRadius, const _vec3 vBoxtMin, const _vec3 vBoxMax, const _matrix matBoxWorld)
			: m_vSpherePos(vSpherePos)
			, m_fSphereRadius(fSphereRadius)
			, m_vBoxMin(vBoxtMin)
			, m_vBoxMax(vBoxMax)
			, m_matBoxWorld(matBoxWorld)
		{
		}
		~Collision_SphereToBox(void) {}
	public:
		//바운딩용 
		_bool operator()()
		{

		}

		_bool operator()(const _vec3 vSpherePos, const _float fSphereRadius, const _vec3 vBoxtMin, const _vec3 vBoxMax, const _matrix matBoxWorld)
		{
			_vec3 vSphereDir = vSpherePos - (_vec3)matBoxWorld.m[3];
			_vec3 vWorldBoxMin, vWorldBoxMax;
			_vec3 vProjSpherePos;
			_vec3 vBoxRight, vBoxUp, vBoxLook;
			_vec3 vCenter;
			_matrix matWorld;


			//vCenter = (vBoxtMin + vBoxMax) * 0.5f;
			//D3DXMatrixTranslation(&matWorld, vCenter.x, vCenter.y, vCenter.z);
			//matWorld *= matBoxWorld;

			D3DXVec3Normalize(&vBoxRight, &_vec3(matBoxWorld.m[0]));
			D3DXVec3Normalize(&vBoxUp, &_vec3(matBoxWorld.m[1]));
			D3DXVec3Normalize(&vBoxLook, &_vec3(matBoxWorld.m[2]));


			//월드상의 박스
			D3DXVec3TransformCoord(&vWorldBoxMax, &vBoxMax, &matBoxWorld);
			D3DXVec3TransformCoord(&vWorldBoxMin, &vBoxtMin, &matBoxWorld);

			_vec3 vBoxSize = {
				D3DXVec3Length(&_vec3(matBoxWorld.m[0])) * (vBoxMax - vBoxtMin).x,
				D3DXVec3Length(&_vec3(matBoxWorld.m[1])) * (vBoxMax - vBoxtMin).y,
				D3DXVec3Length(&_vec3(matBoxWorld.m[2])) * (vBoxMax - vBoxtMin).z };
			vBoxSize *= 0.5f;

			//박스의 축에 투영
			vProjSpherePos = { D3DXVec3Dot(&vSphereDir,&vBoxRight),D3DXVec3Dot(&vSphereDir,&vBoxUp),D3DXVec3Dot(&vSphereDir,&vBoxLook) };

			if (D3DXVec3Length(&vProjSpherePos) > D3DXVec3Length(&vBoxSize) + fSphereRadius)
				return false;

			for (int i = 0; i < 3; ++i)
				if (abs(vProjSpherePos[i]) > vBoxSize[i] + fSphereRadius)
					return false;

			return true;
		}

	private:
		_vec3	m_vSpherePos, m_vBoxMin, m_vBoxMax;
		_float	m_fSphereRadius;
		_matrix m_matBoxWorld;
	};


	class Collision_Box
	{
		typedef	struct tagOBB
		{
			tagOBB()
			{
				ZeroMemory(vPoint, sizeof(_vec3) * 8);
				ZeroMemory(vCenter, sizeof(_vec3));
				ZeroMemory(vProjAxis, sizeof(_vec3) * 3);
				ZeroMemory(vAxis, sizeof(_vec3) * 3);
			}
			_vec3		vPoint[8];
			_vec3		vCenter;
			_vec3		vProjAxis[3];	// 객체 당 세 개의 면을 향해 뻗어나가는 벡터
			_vec3		vAxis[3];		// 박스와 평행한 임의의 축

		}OBB;
	public:
		explicit Collision_Box()
		{
		}
		explicit Collision_Box(_vec3& vDestMin, _vec3& vDestMax, _matrix& matDestWorld,
			_vec3& vSourMin, _vec3& vSourMax, _matrix& matSourWorld)
			: m_vSrcMin(vSourMin)
			, m_vSrcMax(vSourMax)
			, m_vDstMin(vDestMin)
			, m_vDstMax(vDestMax)
			, m_matSrcWorld(matSourWorld)
			, m_matDstWorld(matDestWorld)
			, m_bIsCollision(false)
		{
		}
		~Collision_Box(void) {}
	public:
		//바운딩용 
		_bool operator()()
		{
			OBB		tObb[2];
			ZeroMemory(tObb, sizeof(OBB) * 2);

			Set_Point(&tObb[0], &m_vDstMin, &m_vDstMax);
			Set_Point(&tObb[1], &m_vSrcMin, &m_vSrcMax);

			for (_uint i = 0; i < 8; ++i)
			{
				D3DXVec3TransformCoord(&tObb[0].vPoint[i], &tObb[0].vPoint[i], &m_matDstWorld);
				D3DXVec3TransformCoord(&tObb[1].vPoint[i], &tObb[1].vPoint[i], &m_matSrcWorld);
			}
			D3DXVec3TransformCoord(&tObb[0].vCenter, &tObb[0].vCenter, &m_matDstWorld);
			D3DXVec3TransformCoord(&tObb[1].vCenter, &tObb[1].vCenter, &m_matSrcWorld);

			for (_uint i = 0; i < 2; ++i)
				Set_Axis(&tObb[i]);


			_float	fDistance[3]{};		// 0번 : 첫 번째 obb에서 임의의 축으로 투영을 내린 길이의 합
										// 1번 : 두 번째 obb에서 임의의 축으로 투영을 내린 길이의 합
										// 2번 : 중점 간의 거리를 의미하는 방향 벡터를 임의의 축으로 투영을 내린 길이

			for (_uint i = 0; i < 2; ++i)
			{
				for (_uint j = 0; j < 3; ++j)
				{
					fDistance[0] = fabs(D3DXVec3Dot(&tObb[0].vProjAxis[0], &tObb[i].vAxis[j])) +
						fabs(D3DXVec3Dot(&tObb[0].vProjAxis[1], &tObb[i].vAxis[j])) +
						fabs(D3DXVec3Dot(&tObb[0].vProjAxis[2], &tObb[i].vAxis[j]));

					fDistance[1] = fabs(D3DXVec3Dot(&tObb[1].vProjAxis[0], &tObb[i].vAxis[j])) +
						fabs(D3DXVec3Dot(&tObb[1].vProjAxis[1], &tObb[i].vAxis[j])) +
						fabs(D3DXVec3Dot(&tObb[1].vProjAxis[2], &tObb[i].vAxis[j]));

					fDistance[2] = fabs(D3DXVec3Dot(&(tObb[1].vCenter - tObb[0].vCenter), &tObb[i].vAxis[j]));

					if (fDistance[0] + fDistance[1] < fDistance[2])
						return false;
				}
			}

			return true;
		}

		_bool operator()(const _vec3 * pDestMin, const _vec3 * pDestMax, const _matrix * pDestWorld, const _vec3 * pSourMin, const _vec3 * pSourMax, const _matrix * pSourWorld)
		{
			OBB		tObb[2];
			ZeroMemory(tObb, sizeof(OBB) * 2);

			Set_Point(&tObb[0], pDestMin, pDestMax);
			Set_Point(&tObb[1], pSourMin, pSourMax);

			for (_uint i = 0; i < 8; ++i)
			{
				D3DXVec3TransformCoord(&tObb[0].vPoint[i], &tObb[0].vPoint[i], pDestWorld);
				D3DXVec3TransformCoord(&tObb[1].vPoint[i], &tObb[1].vPoint[i], pSourWorld);
			}
			D3DXVec3TransformCoord(&tObb[0].vCenter, &tObb[0].vCenter, pDestWorld);
			D3DXVec3TransformCoord(&tObb[1].vCenter, &tObb[1].vCenter, pSourWorld);

			for (_uint i = 0; i < 2; ++i)
				Set_Axis(&tObb[i]);


			_float	fDistance[3]{};		// 0번 : 첫 번째 obb에서 임의의 축으로 투영을 내린 길이의 합
										// 1번 : 두 번째 obb에서 임의의 축으로 투영을 내린 길이의 합
										// 2번 : 중점 간의 거리를 의미하는 방향 벡터를 임의의 축으로 투영을 내린 길이

			for (_uint i = 0; i < 2; ++i)
			{
				for (_uint j = 0; j < 3; ++j)
				{
					fDistance[0] = fabs(D3DXVec3Dot(&tObb[0].vProjAxis[0], &tObb[i].vAxis[j])) +
						fabs(D3DXVec3Dot(&tObb[0].vProjAxis[1], &tObb[i].vAxis[j])) +
						fabs(D3DXVec3Dot(&tObb[0].vProjAxis[2], &tObb[i].vAxis[j]));

					fDistance[1] = fabs(D3DXVec3Dot(&tObb[1].vProjAxis[0], &tObb[i].vAxis[j])) +
						fabs(D3DXVec3Dot(&tObb[1].vProjAxis[1], &tObb[i].vAxis[j])) +
						fabs(D3DXVec3Dot(&tObb[1].vProjAxis[2], &tObb[i].vAxis[j]));

					fDistance[2] = fabs(D3DXVec3Dot(&(tObb[1].vCenter - tObb[0].vCenter), &tObb[i].vAxis[j]));

					if (fDistance[0] + fDistance[1] < fDistance[2])
						return false;
				}
			}

			return true;
		}

		void Collision_Box::Set_Point(OBB * pObb, const _vec3 * pMin, const _vec3 * pMax)
		{

			pObb->vPoint[0] = _vec3(pMin->x, pMax->y, pMin->z);
			pObb->vPoint[1] = _vec3(pMax->x, pMax->y, pMin->z);
			pObb->vPoint[2] = _vec3(pMax->x, pMin->y, pMin->z);
			pObb->vPoint[3] = _vec3(pMin->x, pMin->y, pMin->z);

			pObb->vPoint[4] = _vec3(pMin->x, pMax->y, pMax->z);
			pObb->vPoint[5] = _vec3(pMax->x, pMax->y, pMax->z);
			pObb->vPoint[6] = _vec3(pMax->x, pMin->y, pMax->z);
			pObb->vPoint[7] = _vec3(pMin->x, pMin->y, pMax->z);

			pObb->vCenter = (*pMin + *pMax) * 0.5f;

		}
		void Collision_Box::Set_Axis(OBB * pObb)
		{
			pObb->vProjAxis[0] = (pObb->vPoint[2] + pObb->vPoint[5]) * 0.5f - pObb->vCenter;
			pObb->vProjAxis[1] = (pObb->vPoint[0] + pObb->vPoint[5]) * 0.5f - pObb->vCenter;
			pObb->vProjAxis[2] = (pObb->vPoint[7] + pObb->vPoint[5]) * 0.5f - pObb->vCenter;

			pObb->vAxis[0] = pObb->vPoint[2] - pObb->vPoint[3];
			pObb->vAxis[1] = pObb->vPoint[0] - pObb->vPoint[3];
			pObb->vAxis[2] = pObb->vPoint[7] - pObb->vPoint[3];

			for (_uint i = 0; i < 3; ++i)
				D3DXVec3Normalize(&pObb->vAxis[i], &pObb->vAxis[i]);
		}
	private:
		_vec3	m_vSrcMin, m_vSrcMax, m_vDstMin, m_vDstMax;
		_matrix	m_matSrcWorld, m_matDstWorld;
		_bool	m_bIsCollision;
	};


	class Collision_Sphere
	{
	public:
		explicit Collision_Sphere()
		{
		}
		explicit Collision_Sphere(const _vec3 vSrcPos, const _float fSrcRadius, const _vec3 vDstPos, const _float fDstRadius)
			: m_vSrcPos(vSrcPos)
			, m_vDstPos(vDstPos)
			, m_fSrcRadius(fSrcRadius)
			, m_fDstRadius(fDstRadius)
			, m_bIsCollision(false)
		{
		}
		~Collision_Sphere(void) {}
	public:
		//바운딩용 
		_bool operator()()
		{
			_vec3 vDist = m_vDstPos - m_vSrcPos;
			_float fRadiusSum = m_fDstRadius + m_fSrcRadius;

			_float fX = vDist.x;
			_float fY = vDist.y;
			_float fZ = vDist.z;

			_float fDisSum = sqrtf((fX*fX) + (fY*fY) + (fZ*fZ));

			if (fDisSum > fRadiusSum)
				m_bIsCollision = false;
			else
				m_bIsCollision = true;

			return m_bIsCollision;
		}

		_bool operator()(const _vec3 vSrcPos, const _float fSrcRadius, const _vec3 vDstPos, const _float fDstRadius)
		{
			m_vSrcPos = vSrcPos;
			m_vDstPos = vDstPos;

			_vec3 vDist = vDstPos - vSrcPos;
			_float fRadiusSum = fDstRadius + fSrcRadius;

			_float fX = vDist.x;
			_float fY = vDist.y;
			_float fZ = vDist.z;

			_float fDisSum = sqrtf((fX*fX) + (fY*fY) + (fZ*fZ));

			if (fDisSum > fRadiusSum)
				return false;
			else
				return true;

			return true;
		}
		_bool Get_Collsion()
		{
			return m_bIsCollision;
		}


	private:
		_vec3	m_vSrcPos, m_vDstPos;
		_float	m_fSrcRadius, m_fDstRadius;
		_bool	m_bIsCollision;
	};


	//////////////////////개발용///////////////////////////////////
	class CAngleInTwoDir
	{

	public:
		explicit CAngleInTwoDir(_float* pOutAngle, _vec3 vStdDir, _vec3 vCmpDir, AXIS eExcpetAxis, _bool bIsWanaaDegree = true)
			: m_pOutAngle(pOutAngle)
			, m_vStdDir(vStdDir)
			, m_vCmpDir(vCmpDir)
			, m_eExceptAixs(eExcpetAxis)
			, m_bIsWannaDegree(bIsWanaaDegree)
		{
		}
		~CAngleInTwoDir(void) {}
	public:
		//바운딩용 
		_bool operator()()
		{
			_vec3	vOut, vStd, vCmp;
			_float	fFlag = 0.f;
			switch (m_eExceptAixs)
			{
			case AXIS_X:
				m_vStdDir.x = 0.f;
				m_vCmpDir.x = 0.f;
				break;
			case AXIS_Y:
				m_vStdDir.y = 0.f;
				m_vCmpDir.y = 0.f;
				break;
			case AXIS_Z:
				m_vStdDir.z = 0.f;
				m_vCmpDir.z = 0.f;
				break;
			default:
				break;
			}


			D3DXVec3Normalize(&vStd, &m_vStdDir);
			D3DXVec3Normalize(&vCmp, &m_vCmpDir);

			//*m_pOutAngle = acosf(D3DXVec3Dot(&vStd, &vCmp));
			_float fDot = D3DXVec3Dot(&vStd, &vCmp);
			_float fOutAngle = acosf(fDot);
			if (isnan(fOutAngle))
			{
				*m_pOutAngle = 0.f;
			}
			else
			{
				*m_pOutAngle = fOutAngle;

			}
			D3DXVec3Cross(&vOut, &vStd, &vCmp);

			switch (m_eExceptAixs)
			{
			case AXIS_X:
				fFlag = vOut.x;
				break;
			case AXIS_Y:
				fFlag = vOut.y;
				break;
			case AXIS_Z:
				fFlag = vOut.z;
				break;
			default:
				break;
			}

			if (fFlag > 0.f)
			{
				if (m_bIsWannaDegree)
				{
					*m_pOutAngle = D3DXToDegree(*m_pOutAngle);
				}

				return true;
			}
			else
			{
				*m_pOutAngle *= -1;

				if (m_bIsWannaDegree)
				{
					*m_pOutAngle = D3DXToDegree(*m_pOutAngle);
				}

				return false;
			}
		}

		_bool operator()(_float* pOutAngle, _vec3 vStdDir, _vec3 vCmpDir, AXIS eExcpetAxis, _bool bIsWannaDegree = true)
		{
			_vec3 vOut, vStd, vCmp;

			switch (eExcpetAxis)
			{
			case AXIS_X:
				vStdDir.x = 0.f;
				vCmpDir.x = 0.f;
				break;
			case AXIS_Y:
				vStdDir.y = 0.f;
				vCmpDir.y = 0.f;
				break;
			case AXIS_Z:
				vStdDir.z = 0.f;
				vCmpDir.z = 0.f;
				break;
			default:
				break;
			}

			D3DXVec3Normalize(&vStd, &vStdDir);
			D3DXVec3Normalize(&vCmp, &vCmpDir);

			_float fOutAngle = acosf(D3DXVec3Dot(&vStd, &vCmp));
			if (isnan(fOutAngle) == 0)
			{
				*pOutAngle = 0.f;
			}
			else
			{
				*pOutAngle = fOutAngle;

			}

			D3DXVec3Cross(&vOut, &vStd, &vCmp);
			if (vOut.y > 0.f)
			{
				if (bIsWannaDegree)
				{
					*m_pOutAngle = D3DXToDegree(*m_pOutAngle);
				}
				return true;
			}
			*pOutAngle *= -1;

			if (bIsWannaDegree)
			{
				*m_pOutAngle = D3DXToDegree(*m_pOutAngle);
			}

			return false;
		}

	private:
		_vec3	m_vStdDir, m_vCmpDir;
		_float*	m_pOutAngle;
		AXIS	m_eExceptAixs;
		_bool	m_bIsWannaDegree;
	};

	class CRotatedDir
	{
	public:
		explicit CRotatedDir(_float fRotAngle, _vec3* pDir, AXIS eRotAixs)
			: m_fRotAngle(fRotAngle)
			, m_eRotAixs(eRotAixs)
			, m_pDir(pDir)
		{
		}
		~CRotatedDir(void) {}

		_vec3 operator()()
		{
			_matrix matRotation;
			_vec3 vNorDir;
			D3DXMatrixIdentity(&matRotation);
			D3DXVec3Normalize(m_pDir, m_pDir);

			switch (m_eRotAixs)
			{
			case AXIS_X:
				D3DXMatrixRotationX(&matRotation, D3DXToRadian(m_fRotAngle));
				break;
			case AXIS_Y:
				D3DXMatrixRotationY(&matRotation, D3DXToRadian(m_fRotAngle));
				break;
			case AXIS_Z:
				D3DXMatrixRotationZ(&matRotation, D3DXToRadian(m_fRotAngle));
				break;
			default:
				break;
			}


			return *D3DXVec3TransformNormal(m_pDir, m_pDir, &matRotation);
		}

	private:
		_float	m_fRotAngle;
		AXIS	m_eRotAixs;
		_vec3*	m_pDir;

	};

	class CAngleInWorldAxis
	{
	public:
		explicit CAngleInWorldAxis()
			: m_vDir(0.f, 0.f, 0.f)
		{
		}

		explicit CAngleInWorldAxis(_vec3* vDir)
			: m_vDir(*vDir)
		{
		}
		explicit CAngleInWorldAxis(const _vec3* vDir)
			:m_vDir(*vDir)
		{
		}
		~CAngleInWorldAxis(void) {}

		_vec3 operator()()
		{
			// 이 함수는 공간상의 벡터를 x각도 -90 ~ 90, y각도 -180~180, z각도 0으로 표현한다
			_vec3 vNor = *D3DXVec3Normalize(&vNor, &m_vDir);
			_vec3 vAngle = {};

			vAngle.y = atan2f(vNor.x, vNor.z);
			vAngle.x = atan2f(vNor.y, sqrtf(pow(vNor.x, 2) + pow(vNor.z, 2)));

			return _vec3(-D3DXToDegree(vAngle.x), D3DXToDegree(vAngle.y), 0.f);
		}


		_vec3 operator()(const _vec3 * pVector)
		{
			// 이 함수는 공간상의 벡터를 x각도 -90 ~ 90, y각도 -180~180, z각도 0으로 표현한다
			_vec3 vNor = *D3DXVec3Normalize(&vNor, pVector);
			_vec3 vAngle = {};

			vAngle.y = atan2f(vNor.x, vNor.z);
			vAngle.x = atan2f(vNor.y, sqrtf(pow(vNor.x, 2) + pow(vNor.z, 2)));

			return _vec3(-D3DXToDegree(vAngle.x), D3DXToDegree(vAngle.y), 0.f);
		}
	private:
		_vec3	m_vDir;

	};


	class CParabola3D
	{
	public:
		explicit CParabola3D()
			: m_vInitPos(0.f, 0.f, 0.f)
			, m_vInitSpeed(0.f, 0.f, 0.f)
			, m_fStoreTime(0.f)
		{
		}
		explicit CParabola3D(_vec3 vInitPos, _vec3 vInitSpeed, _float fStoreTime)
			: m_vInitPos(vInitPos)
			, m_vInitSpeed(vInitSpeed)
			, m_fStoreTime(fStoreTime)
		{
		}
		~CParabola3D(void) {}

		void	Reset_Functor(_vec3 vInitPos, _vec3 vInitSpeed, _float fStoreTime)
		{
			m_vInitPos = vInitPos;
			m_vInitSpeed = vInitSpeed;
			m_fStoreTime = fStoreTime;
		}
		_vec3 operator()(_float fGravity, _float fTimeDelta)
		{
			_vec3 vOutPos;
			m_fStoreTime += fTimeDelta;
			vOutPos = m_vInitPos;
			vOutPos.x = m_vInitSpeed.x*m_fStoreTime + m_vInitPos.x;
			vOutPos.y = -(0.5f*fGravity*(m_fStoreTime*m_fStoreTime)) + m_vInitSpeed.y*m_fStoreTime + m_vInitPos.y;
			vOutPos.z = m_vInitSpeed.z*m_fStoreTime + m_vInitPos.z;

			return vOutPos;
		}
	private:
		_vec3	m_vInitPos;
		_vec3	m_vInitSpeed;
		_float	m_fStoreTime;
	};

	class CProjectionToPlane
	{
	public:
		explicit CProjectionToPlane()
			: m_vIsProjectedDir(0.f, 0.f, 0.f)
			, m_vPlaneNormal(0.f, 0.f, 0.f)
		{
		}
		explicit CProjectionToPlane(_vec3 vIsProjectedDir, _vec3 vPlaneNormal)
			: m_vIsProjectedDir(vIsProjectedDir)
			, m_vPlaneNormal(vPlaneNormal)
		{
		}
		~CProjectionToPlane(void) {}

		void	Reset_Functor(_vec3 vIsProjectedDir, _vec3 vPlaneNormal)
		{
			m_vIsProjectedDir = vIsProjectedDir;
			m_vPlaneNormal = vPlaneNormal;
		}
		_vec3 operator()()
		{
			_vec3 vOutput;

			vOutput = m_vIsProjectedDir - (D3DXVec3Dot(&m_vIsProjectedDir, &m_vPlaneNormal)*m_vPlaneNormal);

			return vOutput;
		}
		_vec3 operator()(_vec3	vIsProjectedDir, _vec3	vPlaneNormal)
		{
			_vec3 vOutput;

			vOutput = vIsProjectedDir - (D3DXVec3Dot(&vIsProjectedDir, &vPlaneNormal)*vPlaneNormal);

			return vOutput;
		}
	private:
		_vec3	m_vIsProjectedDir;
		_vec3	m_vPlaneNormal;
	};

	class CFrustrum
	{
	public:
		enum CULLDIST { FAR_NORMAL, FAR_HALF, FAR_QUATER };
		explicit CFrustrum()
		{
			ZeroMemory(m_vPoint, sizeof(_vec3) * 8);
			ZeroMemory(m_Plane, sizeof(D3DXPLANE) * 6);
		}


		HRESULT	Set_Point()
		{
			m_vPoint[0] = _vec3(-1.f, 1.f, 0.f);
			m_vPoint[1] = _vec3(1.f, 1.f, 0.f);
			m_vPoint[2] = _vec3(1.f, -1.f, 0.f);
			m_vPoint[3] = _vec3(-1.f, -1.f, 0.f);

			m_vPoint[4] = _vec3(-1.f, 1.f, 1.f);
			m_vPoint[5] = _vec3(1.f, 1.f, 1.f);
			m_vPoint[6] = _vec3(1.f, -1.f, 1.f);
			m_vPoint[7] = _vec3(-1.f, -1.f, 1.f);

			m_vHalfFar[0] = m_vPoint[7];
			m_vHalfFar[1] = m_vPoint[6];
			m_vHalfFar[2] = m_vPoint[5];

			m_vQaurterFar[0] = m_vPoint[7];
			m_vQaurterFar[1] = m_vPoint[6];
			m_vQaurterFar[2] = m_vPoint[5];

			return S_OK;
		}
		HRESULT	Set_Plane(_matrix matView, _matrix matProj)
		{
			_matrix		matInvProj, matInvView, matHalfInvView, matQaurterInvView;
			_vec3		vAdjPoint[3];
			Set_Point();


			matHalfInvView = matView;
			matQaurterInvView = matView;


			D3DXMatrixInverse(&matInvView, NULL, &matView);
			D3DXMatrixInverse(&matInvProj, NULL, &matProj);

			matQaurterInvView = matInvProj;
			matQaurterInvView._44 *= 10.f;
			matQaurterInvView._34 *= 10.f;

			matHalfInvView = matInvProj;
			matHalfInvView._44 *= 10.f;
			matHalfInvView._34 *= 10.f;

			//matQaurterInvView = matInvProj;

			for (_uint i = 0; i < 8; ++i)
			{
				D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matInvProj);
				D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matInvView);
			}

			for (_uint i = 0; i < 3; ++i)
			{
				D3DXVec3TransformCoord(&m_vHalfFar[i], &m_vHalfFar[i], &matHalfInvView);
				D3DXVec3TransformCoord(&m_vHalfFar[i], &m_vHalfFar[i], &matInvView);

				D3DXVec3TransformCoord(&m_vQaurterFar[i], &m_vQaurterFar[i], &matQaurterInvView);
				D3DXVec3TransformCoord(&m_vQaurterFar[i], &m_vQaurterFar[i], &matInvView);
			}

			//////////////////////////////////////Near - Far까지 ///////////////////////
			// +x
			D3DXPlaneFromPoints(&m_Plane[0], &m_vPoint[1], &m_vPoint[5], &m_vPoint[6]);

			// -x
			D3DXPlaneFromPoints(&m_Plane[1], &m_vPoint[4], &m_vPoint[0], &m_vPoint[3]);

			// +y
			D3DXPlaneFromPoints(&m_Plane[2], &m_vPoint[4], &m_vPoint[5], &m_vPoint[1]);

			// -y
			D3DXPlaneFromPoints(&m_Plane[3], &m_vPoint[3], &m_vPoint[2], &m_vPoint[6]);

			// -z
			D3DXPlaneFromPoints(&m_Plane[4], &m_vPoint[0], &m_vPoint[1], &m_vPoint[2]);

			// +z
			D3DXPlaneFromPoints(&m_Plane[5], &m_vPoint[7], &m_vPoint[6], &m_vPoint[5]);

			// +z Half

			D3DXPlaneFromPoints(&m_PlaneHalf, &m_vHalfFar[0], &m_vHalfFar[1], &m_vHalfFar[2]);

			// +z Quarter
			D3DXPlaneFromPoints(&m_PlaneQuarter, &m_vQaurterFar[0], &m_vQaurterFar[1], &m_vQaurterFar[2]);

			return S_OK;
		}

		_bool IsIn_Frustum(const _vec3* pPos, const _float& fRadius, CULLDIST eDist)
		{
			_float		fDistance = 0.f;

			if (eDist == FAR_NORMAL)
			{
				for (_uint i = 0; i < 6; ++i)
				{
					fDistance = D3DXPlaneDotCoord(&m_Plane[i], pPos);
					if (fDistance > fRadius)
						return false;
				}
			}
			else if (eDist == FAR_HALF)
			{
				for (_uint i = 0; i < 5; ++i)
				{
					fDistance = D3DXPlaneDotCoord(&m_Plane[i], pPos);

					if (fDistance > fRadius)
						return false;
				}
				fDistance = D3DXPlaneDotCoord(&m_PlaneHalf, pPos);
				if (fDistance > fRadius)
					return false;
			}
			else if (eDist == FAR_QUATER)
			{
				for (_uint i = 0; i < 5; ++i)
				{
					fDistance = D3DXPlaneDotCoord(&m_Plane[i], pPos);
					if (fDistance > fRadius)
						return false;
				}
				fDistance = D3DXPlaneDotCoord(&m_PlaneQuarter, pPos);
				if (fDistance > fRadius)
					return false;
			}


			return true;
		}

		_bool IsIn_Frustum_ForObject(const _vec3* pPosWorld, const _float& fRadius = 0.f, CULLDIST eDist = FAR_NORMAL)
		{
			return IsIn_Frustum(pPosWorld, fRadius, eDist);
		}
	private:
		_vec3					m_vPoint[8];
		_vec3					m_vHalfFar[3];
		_vec3					m_vQaurterFar[3];

		D3DXPLANE				m_Plane[6];
		D3DXPLANE				m_PlaneHalf;
		D3DXPLANE				m_PlaneQuarter;
	};


	//////////////////////최적화용////////////////////////////



	class CCalc
	{
	public:
		explicit CCalc() {}
		~CCalc(void) {}
	public:
		_bool operator()(Collision_Sphere &Functor)
		{
			return Functor();
		}

		_bool operator()(Collision_Box &Functor)
		{
			return Functor();
		}

		// RETURN TRUE	 STDDIR	- CMPDIR 
		// RETURN FALSE  CMPDIR	- STDDIR 
		_bool operator()(CAngleInTwoDir &Functor)
		{
			return Functor();
		}

		_vec3 operator()(CAngleInWorldAxis &Functor)
		{

			return Functor();
		}
		_vec3 operator()(CRotatedDir &Functor)
		{

			return Functor();
		}
		_vec3 operator()(CProjectionToPlane &Functor)
		{

			return Functor();
		}
	};




	///////////////////////////////////////////////////////////

	static float ComputeGaussianValue(float x, float mean, float std_deviation)
	{
		// The gaussian equation is defined as such:
		/*
		-(x - mean)^2
		-------------
		1.0               2*std_dev^2
		f(x,mean,std_dev) = -------------------- * e^
		sqrt(2*pi*std_dev^2)

		*/

		return (1.0f / sqrt(2.0f * D3DX_PI * std_deviation * std_deviation))
			* expf((-((x - mean) * (x - mean))) / (2.0f * std_deviation * std_deviation));
	}


	struct CFunc
	{
		static _int GetRandomInt(_int iMin, _int iMax)
		{
			if (iMin >= iMax)
				return iMin;

			_int iRand = (rand() % (iMax + 1 - iMin));
			return iMin + iRand;
		}
		static float GetRandomFloat(float fMin, float fMax)
		{
			if (fMin >= fMax)
				return fMin;

			_float f = (rand() % 10000)*0.0001f;
			return (f*(fMax - fMin)) + fMin;
		}
		static D3DXVECTOR3 GetRandomVector(D3DXVECTOR3 vMin, D3DXVECTOR3 vMax)
		{
			D3DXVECTOR3 vVector;
			vVector.x = GetRandomFloat(vMin.x, vMax.x);
			vVector.y = GetRandomFloat(vMin.y, vMax.y);
			vVector.z = GetRandomFloat(vMin.z, vMax.z);

			return vVector;
		}
		static D3DXVECTOR4 GetRandomVector4(D3DXVECTOR4 vMin, D3DXVECTOR4 vMax)
		{
			D3DXVECTOR4 vVector;
			vVector.x = GetRandomFloat(vMin.x, vMax.x);
			vVector.y = GetRandomFloat(vMin.y, vMax.y);
			vVector.z = GetRandomFloat(vMin.z, vMax.z);
			vVector.w = GetRandomFloat(vMin.w, vMax.w);

			return vVector;
		}
		static _float GetMaximumValue(_float pArray[], DWORD dwMaxIndex, DWORD* pIndex = nullptr)
		{
			_float fMaxValue = 0.f;
			for (DWORD dwIndex = 0; dwIndex < dwMaxIndex; ++dwIndex)
			{
				if (pArray[dwIndex] > fMaxValue)
				{
					fMaxValue = pArray[dwIndex];
					if (nullptr != pIndex)
						*pIndex = dwIndex;
				}
			}
			return fMaxValue;
		}
		static _float GetMinimumValue(_float pArray[], DWORD dwMaxIndex, DWORD* pIndex = nullptr)
		{
			_float fMinValue = 99999.f;
			for (DWORD dwIndex = 0; dwIndex < dwMaxIndex; ++dwIndex)
			{
				if (pArray[dwIndex] < fMinValue)
				{
					fMinValue = pArray[dwIndex];
					if (nullptr != pIndex)
						*pIndex = dwIndex;
				}
			}
			return fMinValue;
		}

		static _float Clamp(_float fVelue,_float fMin, _float fMax)
		{
			if (fVelue > fMax)
				fVelue = fMax;
			if (fVelue < fMin)
				fVelue = fMin;
			return fVelue;
		}

		static _vec3 Clamp(_vec3 vVelue, _vec3 vMin, _vec3 vMax)
		{
			vVelue.x = Clamp(vVelue.x, vMin.x, vMax.x);
			vVelue.y = Clamp(vVelue.y, vMin.y, vMax.y);
			vVelue.z = Clamp(vVelue.z, vMin.z, vMax.z);
			return vVelue;
		}
		static _vec2 Clamp(_vec2 vVelue, _vec2 vMin, _vec2 vMax)
		{
			vVelue.x = Clamp(vVelue.x, vMin.x, vMax.x);
			vVelue.y = Clamp(vVelue.y, vMin.y, vMax.y);
			return vVelue;
		}

		///////////////////////////////////////////////////
	
		static _int Collision_Plane_To_Sphere(_vec3* pOut, D3DXPLANE tPlane,_vec3 vPlaneNormal,_vec3 vSphereCenter,_float fSphereRadius)
		{
			_float fDist = D3DXVec3Dot(&vSphereCenter, &vPlaneNormal) + tPlane.d;
		
				*pOut = vPlaneNormal * fDist;
			if (fabsf(fDist) < fSphereRadius)
			{
		
			}
			else
			{
				*pOut = {0.f,0.f,0.f};
				//평면보다 위에 있다
				if (fDist > fSphereRadius)
					return Plane_On;
		
				//평면보다 아래에 있다
				if (fDist < fSphereRadius)
					return Plane_Under;
			}
		//평면과 겹쳐있다
			return Plane_Cross;
		}
	};
}




#endif // ENGINE_Function_h__
