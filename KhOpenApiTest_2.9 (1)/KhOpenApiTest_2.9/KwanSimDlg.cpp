//******************************************************************/
//******************************************************************/
//! All Rights Reserved. Copyright(c)   2014 (주)한국소리마치        /
//******************************************************************/
//! File Name     : KwanSimDlg.cpp
//! Function      : 복수종목 다이얼로그
//! System Name   : 키움 오픈API 테스트
//! Create        : , 2014/06/02
//! Update        : 
//! Comment       : 
//******************************************************************/
// KwanSimDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "KhOpenApiTest.h"
#include "KwanSimDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const CString m_strRealSet = "주식시세;주식체결";

// {조회 키,		리얼 키,	행, 열, 타입,			색 변경, 정렬, 앞 문자, 뒷 문자}
const stGRID lstOPTKWFID[] = 
{
	{_T("종목코드"),		_T("-1"),	-1,	0,	DT_NONE,		FALSE,	DT_LEFT,	_T(""),	_T("")}, 
	{_T("종목명"),		_T("-1"),	-1,	1,	DT_NONE,		FALSE,	DT_LEFT,	_T(""),	_T("")}, 
	{_T("현재가"),		_T("0"),	-1,	2,	DT_ZERO_NUMBER,	TRUE,	DT_RIGHT,	_T(""),	_T("")}, 
	{_T("전일대비기호"),	_T("10"),	-1,	3,	DT_SIGN,		TRUE,	DT_CENTER,	_T(""),	_T("")}, 
	{_T("전일대비"),		_T("1"),	-1,	4,	DT_ZERO_NUMBER,	TRUE,	DT_RIGHT,	_T(""),	_T("")}, 
	{_T("등락율"),		_T("2"),	-1,	5,	DT_ZERO_NUMBER,	TRUE,	DT_RIGHT,	_T(""),	_T("%")}, 
	{_T("거래량"),		_T("5"),	-1,	6,	DT_ZERO_NUMBER,	FALSE,	DT_RIGHT,	_T(""),	_T("")}, 
	{_T("전일거래량대비"),	_T("13"),	-1,	7,	DT_ZERO_NUMBER,	TRUE,	DT_RIGHT,	_T(""),	_T("%")}, 
};

// 실시간 주문체결(현재가 표시용)
const stGRID lstOPTKWFID_B[] = 
{
	{_T("종목코드"),		_T("-1"),	-1,	0,	DT_NONE,		FALSE,	DT_LEFT,	_T(""),	_T("")}, 
	{_T("종목명"),		_T("-1"),	-1,	1,	DT_NONE,		FALSE,	DT_LEFT,	_T(""),	_T("")}, 
	{_T("현재가"),		_T("1"),	-1,	2,	DT_ZERO_NUMBER,	TRUE,	DT_RIGHT,	_T(""),	_T("")}, 
	{_T("전일대비기호"),	_T("12"),	-1,	3,	DT_SIGN,		TRUE,	DT_CENTER,	_T(""),	_T("")}, 
	{_T("전일대비"),		_T("2"),	-1,	4,	DT_ZERO_NUMBER,	TRUE,	DT_RIGHT,	_T(""),	_T("")}, 
	{_T("등락율"),		_T("3"),	-1,	5,	DT_ZERO_NUMBER,	TRUE,	DT_RIGHT,	_T(""),	_T("%")}, 
	{_T("거래량"),		_T("7"),	-1,	6,	DT_ZERO_NUMBER,	FALSE,	DT_RIGHT,	_T(""),	_T("")}, 
	{_T("전일거래량대비"),	_T("15"),	-1,	7,	DT_ZERO_NUMBER,	TRUE,	DT_RIGHT,	_T(""),	_T("%")}, 
};

CKwanSimDlg::CKwanSimDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CKwanSimDlg::IDD, pParent)
{
	m_pParent = pParent;
	m_strScrNo = _T("9999");
	m_cellSelect.row = 0;
	m_cellSelect.col = 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKwanSimDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKwanSimDlg)
	DDX_Control(pDX, IDC_GRD_KWANSIM,		m_grdKwanSim);
	//}}AFX_DATA_MAP
}

//*******************************************************************/
// BEGIN_MESSAGE_MAP
//*******************************************************************/
BEGIN_MESSAGE_MAP(CKwanSimDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_JONGADD,			OnBtnJongAdd)
	ON_BN_CLICKED(IDC_BTN_JONGDELETE,		OnBtnJongDelete)
	ON_BN_CLICKED(IDC_BTN_JONGALLDELETE,	OnBtnJongAllDelete)
	ON_BN_CLICKED(IDC_BTN_COMMKW_RQ,		OnBtnCommKwRq)
END_MESSAGE_MAP()

//*******************************************************************/
//! Function Name : OnInitDialog
//! Function      : 생성 초기 처리
//! Param         : void
//! Return        : BOOL
//! Create        : , 2014/06/02
//! Comment       : CKwanSimDlg 메시지 처리기
//******************************************************************/
BOOL CKwanSimDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	((CEdit*)GetDlgItem(IDC_EDT_JONGCODE))->SetLimitText(6);	// 종목코드 최대길이 설정

	InitMultiCodeGrid();					// 복수종목 그리드 초기 처리

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CKwanSimDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

void CKwanSimDlg::OnClose()
{
	// 화면 닫을 때 부모윈도우에게 통보(화면 관리 위해)
	if (m_pParent)
	{
		int nLen = m_strScrNo.GetLength();
		char *cScrNo = new char[nLen + 1];
		memset(cScrNo, 0x00, nLen + 1);
		memcpy(cScrNo, m_strScrNo, nLen);
		m_pParent->PostMessage(UM_SCRENN_CLOSE, 0U, (LPARAM)cScrNo);
	}

	CDialogEx::OnClose();
}

HCURSOR CKwanSimDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//*******************************************************************/
//! Function Name : OnBtnJongAdd
//! Function      : 종목추가 버튼
//! Param         : void
//! Return        : void
//! Create        : , 2014/06/02
//! Comment       : 
//******************************************************************/
void CKwanSimDlg::OnBtnJongAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 종목코드 입력 여부
	CString strCode, strIndex;
	((CEdit*)GetDlgItem(IDC_EDT_JONGCODE))->GetWindowText(strCode);
	if (strCode.GetLength() != 6)
	{
		AfxMessageBox(_T("종목코드 6자를 입력 해 주세요~!"));
		((CEdit*)GetDlgItem(IDC_EDT_JONGCODE))->SetFocus();
		return;
	}

	if (m_mapJongCode.Lookup(strCode, strIndex))
	{
		AfxMessageBox(_T("이미 등록 된 종목입니다."));
		CCellID cureentCell(atoi(strIndex), 0);
		m_grdKwanSim.SetFocusCell(cureentCell);
		m_grdKwanSim.SetSelectedRange(cureentCell.row, 0, cureentCell.row, 7);
		((CEdit*)GetDlgItem(IDC_EDT_JONGCODE))->SetFocus();
		return;
	}

	CString strFileName = theApp.m_sAppPath + _T("/data/kwansim.ini");
	int nCnt = ::GetPrivateProfileInt(_T("JONG_CODE"), _T("COUNT"), 0,strFileName) + 1;

	// 종목코드 갯수 추가
	strIndex.Format("%d", nCnt);
	::WritePrivateProfileString(_T("JONG_CODE"), _T("COUNT"), strIndex, strFileName);

	// 종목코드 추가
	::WritePrivateProfileString(_T("JONG_CODE"), strIndex, strCode, strFileName);
	m_mapJongCode.SetAt(strCode, strIndex);

	// 행 추가
	m_grdKwanSim.InsertRow(_T(""), -1);
	m_grdKwanSim.SetRowHeight(1, 20);		// 행의 높이 설정
	m_grdKwanSim.SetItemFormat(lstOPTKWFID[nCnt].nRow, lstOPTKWFID[nCnt].nCol, lstOPTKWFID[nCnt].nAlign);
	m_grdKwanSim.SetItemText(nCnt, 0, strCode);

	// 종목 조회 요청
	SendJongSearch(1, strCode, 5);
}

//*******************************************************************/
//! Function Name : OnBtnJongDelete
//! Function      : 종목삭제 버튼
//! Param         : void
//! Return        : void
//! Create        : , 2014/06/02
//! Comment       : 
//******************************************************************/
void CKwanSimDlg::OnBtnJongDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nCnt = m_grdKwanSim.GetRowCount();
	if (nCnt > 2)
	{
		m_cellSelect = m_grdKwanSim.GetFocusCell();
		if (m_cellSelect.row < 1 || m_cellSelect.row > nCnt - 1)
		{
			return;
		}
		CString strTemp;
		strTemp.Format(_T("종목 [%s %s] 을 삭제 하겠습니까?"), 
			m_grdKwanSim.GetItemText(m_cellSelect.row, 0), 
			m_grdKwanSim.GetItemText(m_cellSelect.row, 1));
		if (MessageBox(strTemp, _T("복수종목 삭제"), MB_ICONQUESTION | MB_YESNO) == IDNO)
		{
			return;
		}

		nCnt--;
		m_grdKwanSim.DeleteRow(m_cellSelect.row);
		m_mapJongCode.RemoveAll();

		CString strFileName = theApp.m_sAppPath + _T("/data/kwansim.ini");
		// 종목코드 갯수 추가
		CString strIndex, strCode;
		strIndex.Format("%d", nCnt - 1);
		::WritePrivateProfileString(_T("JONG_CODE"), _T("COUNT"), strIndex, strFileName);

		// 종목 재설정
		for (auto i = 1; i < nCnt; i++)
		{
			strIndex.Format(_T("%d"), i);
			strCode = m_grdKwanSim.GetItemText(i, 0);

			// 종목코드 추가
			::WritePrivateProfileString(_T("JONG_CODE"), strIndex, strCode, strFileName);
			m_mapJongCode.SetAt(strCode, strIndex);
		}

		SendJongSearch();		// 종목 조회 요청
	}
	else
	{
		OnBtnJongAllDelete();
	}
}

//*******************************************************************/
//! Function Name : OnBtnJongAllDelete
//! Function      : 전체삭제 버튼
//! Param         : void
//! Return        : void
//! Create        : , 2014/06/02
//! Comment       : 
//******************************************************************/
void CKwanSimDlg::OnBtnJongAllDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (MessageBox(_T("복수종목을 전체삭제 하겠습니까?"), _T("복수종목 전체삭제"), MB_ICONQUESTION | MB_YESNO) == IDNO)
	{
		return;
	}
	// 파일삭제
	CString strFileName = theApp.m_sAppPath + _T("/data/kwansim.ini");
	::DeleteFile(strFileName);
	m_mapJongCode.RemoveAll();		// 종목코드 초기화
	m_grdKwanSim.SetRowCount(1);	// 그리드 초기화

	m_grdKwanSim.Invalidate();
}

//*******************************************************************/
//! Function Name : OnBtnSearch
//! Function      : 조회 버튼
//! Param         : void
//! Return        : void
//! Create        : , 2014/06/02
//! Comment       : 
//******************************************************************/
void CKwanSimDlg::OnBtnCommKwRq()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_cellSelect.row = 0;
	m_cellSelect.col = 0;
	m_grdKwanSim.SetRowCount(1);
	SetMultiCode();					// 복수 그리드에 종목 설정 처리
}

//*******************************************************************/
//! Function Name : InitKwanSimGrid
//! Function      : 호가 그리드 초기 처리
//! Param         : void
//! Return        : void
//! Create        : , 2014/06/02
//! Comment       : 
//******************************************************************/
void CKwanSimDlg::InitMultiCodeGrid()
{
	m_grdKwanSim.SetEditable(false);				//cell을 에디트 못하게 함.
	m_grdKwanSim.EnableScrollBars(SB_BOTH, FALSE);

	COLORREF clr = RGB(215, 227, 241);

	// 고정 행/열 설정
	m_grdKwanSim.SetFixedRowCount(1);

	// 행/열 갯수 설정
	m_grdKwanSim.SetRowCount(1);
	m_grdKwanSim.SetColumnCount(8);

	// 열의 넓이 설정
	int i, nWidth[] = {50, 80, 80, 20, 60, 60, 80, 80};
	CString strHeader[] = {_T("코드"), _T("종목명"), _T("현재가"), _T(""), _T("전일대비"), _T("등락율"), _T("거래량"), _T("거래량대비")};
	int nCnt = sizeof(nWidth) / sizeof(*nWidth);		// 전체크기 / 원소크기 = 원소개수
	for (i = 0; i < nCnt; i++)
	{
		m_grdKwanSim.SetColumnWidth(i, nWidth[i]);
		m_grdKwanSim.SetItemFormat(0, i, DT_CENTER);
		m_grdKwanSim.SetItemText(0, i, strHeader[i]);
		m_grdKwanSim.SetItemBkColour(0, i, clr);	// 지정된 셀의 배경색 설정
	}

	// 행의 높이 설정
	m_grdKwanSim.SetRowHeight(0, 24);

	m_grdKwanSim.Invalidate();
}

//*******************************************************************/
//! Function Name : SetKwanSimJong
//! Function      : 복수종목 그리드에 종목 설정 처리
//! Param         : void
//! Return        : void
//! Create        : , 2014/06/02
//! Comment       : 
//******************************************************************/
void CKwanSimDlg::SetMultiCode()
{
	CString strFileName = theApp.m_sAppPath + _T("/data/kwansim.ini");
	int nCnt = ::GetPrivateProfileInt(_T("JONG_CODE"), _T("COUNT"), 0, strFileName) + 1;

	char szItem[12];
	int nSize = sizeof(szItem);
	CString strCode, strIndex, strCodeList = _T("");
	m_grdKwanSim.SetRowCount(nCnt);
	m_mapJongCode.RemoveAll();
	for (int i = 1; i < nCnt; i++)
	{
		strIndex.Format("%d", i);
		memset(szItem, 0, nSize);
		::GetPrivateProfileString(_T("JONG_CODE"), strIndex, _T(""), szItem, nSize, strFileName);
		strCode = szItem;		strCode.Trim();

		// 종목코드 추가
		m_mapJongCode.SetAt(strCode, strIndex);

		m_grdKwanSim.SetRowHeight(i, 20);		// 행의 높이 설정
		m_grdKwanSim.SetItemText(i, 0, strCode);

		strCodeList += strCode + _T(";");
	}

	if (!strCodeList.IsEmpty())
	{
		// 종목 조회 요청
		SendJongSearch(nCnt - 1, strCodeList);
	}

	m_grdKwanSim.Invalidate();
}

//*******************************************************************/
//! Function Name : SendJongSearch
//! Function      : 종목 조회 요청
//! Param         : int nCodeCount/* = 0*/, CString strCodeList/* = _T("")*/ : 유형 - "000660;005930;", int nAddType/*= 0*/ 기존 종목조회에서 추가로 종목을 조회할때 여부(0:기본, 5:추가)
//! Return        : void
//! Create        : , 2014/06/02
//! Comment       : 
//******************************************************************/
void CKwanSimDlg::SendJongSearch(int nCodeCount/* = 0*/, CString strCodeList/* = _T("")*/, int nAddType/*= 0*/)
{
	if (nCodeCount < 1 || strCodeList.GetLength() < 1)
	{
		int nCnt = m_grdKwanSim.GetRowCount();
		if (nCnt < 2)
		{
			return;
		}
		nCodeCount = 0;
		for (int i = 1; i < nCnt; i++)
		{
			nCodeCount++;
			strCodeList += m_grdKwanSim.GetItemText(i, 0) + _T(";");
		}
	}

	if (nCodeCount > 0 && strCodeList.GetLength() > 0)
	{
		CString strRQName = _T("복수종목");
		long lRet = theApp.m_khOpenApi.CommKwRqData(strCodeList, 0, nCodeCount, nAddType, strRQName, m_strScrNo);
		if (!theApp.IsError(lRet))
		{
			return;
		}
	}
}

//*******************************************************************/
//! Function Name : OnReceiveTrDataKhopenapictrl
//! Function      : 조회 응답 처리
//! Param         : LPCTSTR sScrNo, LPCTSTR sRQName, LPCTSTR sTrcode, LPCTSTR sRecordName, LPCTSTR sPrevNext, long nDataLength, LPCTSTR sErrorCode, LPCTSTR sMessage, LPCTSTR sSplmMsg
//! Return        : void
//! Create        : , 2014/06/02
//! Comment       : 
//******************************************************************/
void CKwanSimDlg::OnReceiveTrDataKhopenapictrl(LPCTSTR sScrNo, LPCTSTR sRQName, LPCTSTR sTrcode, LPCTSTR sRecordName, LPCTSTR sPrevNext, long nDataLength, LPCTSTR sErrorCode, LPCTSTR sMessage, LPCTSTR sSplmMsg)
{
	CString strRQName = sRQName;
	if (strRQName == _T("복수종목"))			// 복수종목 조회
	{
		CString strData;
		CStringArray arrData;
		int nFieldCnt = sizeof(lstOPTKWFID) / sizeof(*lstOPTKWFID);		// 전체크기 / 원소크기 = 원소개수

		strRQName = _T("관심종목정보");
		int nCnt = theApp.m_khOpenApi.GetRepeatCnt(sTrcode, strRQName);
		for (int i = 0; i < nCnt; i++)
		{
			arrData.RemoveAll();
			for (int j = 0; j < nFieldCnt; j++)
			{
				strData = theApp.m_khOpenApi.GetCommData(sTrcode, strRQName, i, lstOPTKWFID[j].strKey);	strData.Trim();
				m_grdKwanSim.SetItemFormat(i + 1, lstOPTKWFID[j].nCol, lstOPTKWFID[j].nAlign);
				arrData.Add(strData);
			}
			SetDataKwanSimGrid(arrData);
		}
		if (m_cellSelect.row > 0)
		{
			if (m_cellSelect.row >= m_grdKwanSim.GetRowCount())
			{
				m_cellSelect.row = m_grdKwanSim.GetRowCount() - 1;
			}
			m_grdKwanSim.SetFocusCell(m_cellSelect);
			m_grdKwanSim.SetSelectedRange(m_cellSelect.row, m_cellSelect.col, m_cellSelect.row, m_cellSelect.col);
		}
	}
}

void CKwanSimDlg::OnReceiveMsgKhopenapictrl(LPCTSTR sScrNo, LPCTSTR sRQName, LPCTSTR sTrCode, LPCTSTR sMsg)
{
}

//*******************************************************************/
//! Function Name : OnReceiveRealDataKhopenapictrl
//! Function      : 실시간 처리
//! Param         : LPCTSTR sJongmokCode, LPCTSTR sRealType, LPCTSTR sRealData
//! Return        : void
//! Create        : , 2014/06/02
//! Comment       : 
//******************************************************************/
void CKwanSimDlg::OnReceiveRealDataKhopenapictrl(LPCTSTR sJongmokCode, LPCTSTR sRealType, LPCTSTR sRealData)
{
	CString strIndex;
	if (!m_mapJongCode.Lookup(sJongmokCode, strIndex) || m_strRealSet.Find(sRealType) < 0)
	{
		return;
	}

	CString strData;
	CStringArray arrData;
	if (!strcmp(sRealType, _T("주식시세")))		// 주식시세
	{
		arrData.Add(sJongmokCode);
		int i, nFieldCnt = sizeof(lstOPTKWFID) / sizeof(*lstOPTKWFID);		// 전체크기 / 원소크기 = 원소개수
		for (i = 1; i < nFieldCnt; i++)
		{
			if (atoi(lstOPTKWFID[i].strRealKey) < 0)
			{
				arrData.Add(_T(""));
				continue;
			}
			strData = theApp.m_khOpenApi.GetCommRealData(sJongmokCode, atoi(lstOPTKWFID[i].strRealKey));	strData.Trim();
			arrData.Add(strData);
		}
		SetDataKwanSimGrid(arrData, sRealType);
	}
	else if (!strcmp(sRealType, _T("주식체결")))	// 주식체결
	{
		arrData.Add(sJongmokCode);
		int i, nFieldCnt = sizeof(lstOPTKWFID_B) / sizeof(*lstOPTKWFID_B);		// 전체크기 / 원소크기 = 원소개수
		for (i = 1; i < nFieldCnt; i++)
		{
			if (atoi(lstOPTKWFID[i].strRealKey) < 0)
			{
				arrData.Add(_T(""));
				continue;
			}
			strData = theApp.m_khOpenApi.GetCommRealData(sJongmokCode, atoi(lstOPTKWFID_B[i].strRealKey));	strData.Trim();
			arrData.Add(strData);
		}
		SetDataKwanSimGrid(arrData, sRealType);
	}
}

void CKwanSimDlg::OnReceiveChejanData(LPCTSTR sGubun, LONG nItemCnt, LPCTSTR sFidList)
{

}

//*******************************************************************/
//! Function Name : SetDataKwanSimGrid
//! Function      : 복수종목 그리드 데이타 설정
//! Param         : CStringArray &arrData, CString strRealType/* = _T("")*/
//! Return        : void
//! Create        : , 2014/06/02
//! Comment       : 
//******************************************************************/
void CKwanSimDlg::SetDataKwanSimGrid(CStringArray &arrData, CString strRealType/* = _T("")*/)
{
	// 종목코드에 맞는 행 찾기
	CString strData, strTemp;
	strData = arrData.GetAt(0);
	if (!m_mapJongCode.Lookup(strData, strTemp))
	{
		return;
	}

	// 종목명
	int i, nRow = atol(strTemp), nCnt = arrData.GetSize();
	if (strRealType == _T(""))
	{
		CString strName = arrData.GetAt(1);
		((CStatic*)GetDlgItem(IDC_EDT_JONGCODE))->GetWindowText(strTemp);	// 종목코드 입력란
		if (strTemp == strData)
		{
			((CStatic*)GetDlgItem(IDC_STC_JONGNAME))->SetWindowText(strName);
		}
		m_grdKwanSim.SetItemText(nRow, lstOPTKWFID[1].nCol, theApp.ConvDataFormat(lstOPTKWFID[1].nDataType, strName, lstOPTKWFID[1].strBeforeData, lstOPTKWFID[1].strAfterData));
	}

	// 데이타 설정
	CString strTempData;
	for (i = 2; i < nCnt; i++)
	{
		strData = arrData.GetAt(i);
		/////////////////////////////////////////////
		// 변경 된 데이터 선택 처리↓↓↓↓↓↓↓↓↓
		if (strRealType != _T("") && (i == 2 || i == 4 || i == 5))
		{
			strTempData = strData;
			strTempData.Replace(_T("+"), _T(""));	strTempData.Replace(_T("-"), _T(""));	strTempData.Replace(_T("."), _T(""));

			strTemp = m_grdKwanSim.GetItemText(nRow, lstOPTKWFID[i].nCol);
			strTemp.Replace(_T("%"), _T(""));	strTemp.Replace(_T("+"), _T(""));	strTemp.Replace(_T("-"), _T(""));
			strTemp.Replace(_T(","), _T(""));	strTemp.Replace(_T("."), _T(""));
			if (strTempData != strTemp)
			{
				m_cellSelect.col = lstOPTKWFID[i].nCol;
				m_grdKwanSim.SetFocusCell(m_cellSelect);
				m_grdKwanSim.SetSelectedRange(nRow, m_cellSelect.col, nRow, m_cellSelect.col);
				m_grdKwanSim.Invalidate();
			}
		}
		// 변경 된 데이터 선택 처리↑↑↑↑↑↑↑↑↑
		/////////////////////////////////////////////
		if (lstOPTKWFID[i].bTextColor)
		{
			strTemp = arrData.GetAt(3);
			if (strTemp == _T("1") || strTemp == _T("2"))	// 부호에 따라 색상변경
			{
				strTemp = _T("1");
			}
			else if (strTemp == _T("4") || strTemp == _T("5"))	// 부호에 따라 색상변경
			{
				strTemp = _T("-1");
			}
			else
			{
				strTemp = _T("0");
			}
			theApp.SetDataFgColour(&m_grdKwanSim, nRow, lstOPTKWFID[i].nCol, strTemp);
		}
		if (lstOPTKWFID[i].nDataType == DT_SIGN)
		{
			theApp.SetSignData(&m_grdKwanSim, nRow, lstOPTKWFID[i].nCol, strData);
		}
		else
		{
			if (strData.IsEmpty())
			{
				continue;
			}
			m_grdKwanSim.SetItemText(nRow, lstOPTKWFID[i].nCol, theApp.ConvDataFormat(lstOPTKWFID[i].nDataType, strData, lstOPTKWFID[i].strBeforeData, lstOPTKWFID[i].strAfterData));
		}
	}
	m_grdKwanSim.Invalidate();
}