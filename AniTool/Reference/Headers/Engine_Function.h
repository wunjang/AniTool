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
			
			D3DXVec3TransformCoord(&m_vBoxMax, &m_vBoxMax, &m_matBoxWorld);
			D3DXVec3TransformCoord(&m_vBoxMin, &m_vBoxMin, &m_matBoxWorld);

			if (m_vSpherePos.x < m_vBoxMin.x && m_vBoxMin.x - m_vSpherePos.x > m_fSphereRadius)
				return false;

			if (m_vSpherePos.x > m_vBoxMax.x && m_vSpherePos.x - m_vBoxMax.x > m_fSphereRadius)
				return false;

			if (m_vSpherePos.y < m_vBoxMin.y && m_vBoxMin.y - m_vSpherePos.y > m_fSphereRadius)
				return false;

			if (m_vSpherePos.y > m_vBoxMax.y && m_vSpherePos.y - m_vBoxMax.y > m_fSphereRadius)
				return false;

			if (m_vSpherePos.z < m_vBoxMin.z && m_vBoxMin.z - m_vSpherePos.z > m_fSphereRadius)
				return false;

			if (m_vSpherePos.z > m_vBoxMax.z && m_vSpherePos.z - m_vBoxMax.z > m_fSphereRadius)
				return false;

			return true;
		}

		_bool operator()(const _vec3 vSpherePos, const _float fSphereRadius, const _vec3 vBoxtMin, const _vec3 vBoxMax, const _matrix matBoxWorld)
		{
			_vec3 vSphereDir = vSpherePos - (_vec3)matBoxWorld.m[3];
			_vec3 vWorldBoxMin, vWorldBoxMax;
			_vec3 vProjSpherePos;
			_vec3 vBoxRight, vBoxUp, vBoxLook;

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
			//vProjSpherePos = { D3DXVec3Dot(&vSpherePos,&vBoxRight),D3DXVec3Dot(&vSpherePos,&vBoxUp),D3DXVec3Dot(&vSphe3rePos,&vBoxLook) };
			vProjSpherePos = { D3DXVec3Dot(&vSphereDir,&vBoxRight),D3DXVec3Dot(&vSphereDir,&vBoxUp),D3DXVec3Dot(&vSphereDir,&vBoxLook) };


			//if (vProjSpherePos.x < vWorldBoxMin.x && vWorldBoxMin.x - vProjSpherePos.x > fSphereRadius)
			//	return false;
			//
			//if (vProjSpherePos.x > vWorldBoxMax.x && vProjSpherePos.x - vWorldBoxMax.x > fSphereRadius)
			//	return false;
			//
			//if (vProjSpherePos.y < vWorldBoxMin.y && vWorldBoxMin.y - vProjSpherePos.y > fSphereRadius)
			//	return false;
			//
			//if (vProjSpherePos.y > vWorldBoxMax.y && vProjSpherePos.y - vWorldBoxMax.y > fSphereRadius)
			//	return false;
			//
			//if (vProjSpherePos.z < vWorldBoxMin.z && vWorldBoxMin.z - vProjSpherePos.z > fSphereRadius)
			//	return false;
			//
			//if (vProjSpherePos.z > vWorldBoxMax.z && vProjSpherePos.z - vWorldBoxMax.z > fSphereRadius)
			//	return false;

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
			_vec3		vPoint[8];
			_vec3		vCenter;
			_vec3		vProjAxis[3];	// 객체 당 세 개의 면을 향해 뻗어나가는 벡터
			_vec3		vAxis[3];		// 박스와 평행한 임의의 축

		}OBB;
	public:
		explicit Collision_Box()
		{
		}
		explicit Collision_Box(_vec3& vDestMin, _vec3& vDestMax,  _matrix& matDestWorld,
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


	/////////////////////////////////////////////////////////
	class CAngleInTwoDir
	{
		
	public:
		explicit CAngleInTwoDir(_float* pOutAngle, _vec3 vStdDir, _vec3 vCmpDir, AXIS eExcpetAxis,_bool bIsWanaaDegree=true)
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

		_bool operator()(_float* pOutAngle, _vec3 vStdDir, _vec3 vCmpDir, AXIS eExcpetAxis,_bool bIsWannaDegree = true)
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
		explicit CAngleInWorldAxis(_vec3* vDir)
			: m_pDir(vDir)
		{
		}
		~CAngleInWorldAxis(void) {}

		_vec3 operator()()
		{
			// 이 함수는 공간상의 벡터를 x각도 -90 ~ 90, y각도 -180~180, z각도 0으로 표현한다
			_vec3 vNor = *D3DXVec3Normalize(&vNor, m_pDir);
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
		_vec3*	m_pDir;

	};


	class CParabola3D
	{
	public:
		explicit CParabola3D()
			: m_vInitPos(0.f,0.f,0.f)
			, m_vInitSpeed(0.f, 0.f, 0.f)
			, m_fStoreTime(0.f)
		{
		}
		explicit CParabola3D(_vec3 vInitPos,_vec3 vInitSpeed, _float fStoreTime)
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





	class CCalc
	{
	public:
		explicit CCalc(){}
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

			float f = (rand() % 10000)*0.0001f;
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
		static float GetMaximumValue(float pArray[], DWORD dwMaxIndex, DWORD* pIndex = nullptr)
		{
			float fMaxValue = 0.f;
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
		static float GetMinimumValue(float pArray[], DWORD dwMaxIndex, DWORD* pIndex = nullptr)
		{
			float fMinValue = 99999.f;
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
	};
}




#endif // ENGINE_Function_h__
