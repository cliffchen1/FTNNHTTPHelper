
// FTNNHTTPLogHelperDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FTNNHTTPLogHelper.h"
#include "FTNNHTTPLogHelperDlg.h"
#include "afxdialogex.h"
#include <afxinet.h>
#include <afxmsg_.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFTNNHTTPLogHelperDlg �Ի���



CFTNNHTTPLogHelperDlg::CFTNNHTTPLogHelperDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFTNNHTTPLogHelperDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CFTNNHTTPLogHelperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFTNNHTTPLogHelperDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &CFTNNHTTPLogHelperDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_DOWNLOAD, &CFTNNHTTPLogHelperDlg::OnBnClickedDownload)
	ON_EN_CHANGE(IDC_URLEDIT, &CFTNNHTTPLogHelperDlg::OnEnChangeUrledit)
END_MESSAGE_MAP()


// CFTNNHTTPLogHelperDlg ��Ϣ�������

BOOL CFTNNHTTPLogHelperDlg::OnInitDialog()
{

	

	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CFTNNHTTPLogHelperDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFTNNHTTPLogHelperDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CFTNNHTTPLogHelperDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CFTNNHTTPLogHelperDlg::HTTPGetFile(const CString strURL, const CString strSavePath)
{
	if (strURL.IsEmpty())
		return FALSE;
	if (strSavePath.IsEmpty())
		return FALSE;
	unsigned short nPort;//�������˿�
	CString strServer, strObject;//�����������Լ��ļ�����
	DWORD dwServiceType, dwRet;
	if (!AfxParseURL(strURL, dwServiceType, strServer, strObject, nPort))
	{
		return FALSE;
	}
	CInternetSession pSession;
	CHttpConnection *pConnect = pSession.GetHttpConnection(strServer, nPort);
	
	if (pConnect == NULL)
	{
		pSession.Close();
		return FALSE;
	}
	CHttpFile *pFile = pConnect->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject);
	if (pFile == NULL)
	{
		pSession.Close();
		delete pConnect;
		pConnect = NULL;
		return FALSE;
	}
	pFile->SendRequest();
	pFile->QueryInfoStatusCode(dwRet);
	if (dwRet != HTTP_STATUS_OK)
	{
		pSession.Close();
		delete pConnect;
		pConnect = NULL;
		return FALSE;
	}
	UINT fileLen = (UINT)pFile->GetLength();
	DWORD dwRead = 1;
	CHAR *szBuffer = new CHAR[fileLen + 1];
	try{
		CString strLocalPath;
		strLocalPath.Format(_T("%s%s%s"), strSavePath, _T("\\"), pFile->GetFileName());
		CFile localFile(strLocalPath, CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive);
		while (dwRead)
		{
			memset(szBuffer, 0, (size_t)(fileLen + 1));
			//��ȡ��������
			dwRead = pFile->Read(szBuffer, fileLen);
			//д�뵽�ļ�
			localFile.Write(szBuffer, dwRead);
		}
		localFile.Close();
		delete[]szBuffer;
		delete pFile;
		delete pConnect;
		pSession.Close();
	}
	catch (CFileException e)
	{
		delete[]szBuffer;
		delete pFile;
		delete pConnect;
		pSession.Close();
		return FALSE;
	}
	return TRUE;
}
CString CFTNNHTTPLogHelperDlg::GetLocalFilePath()
{
	//
	char szPath[10000];
	CString strLocalDir = CString(szPath);
	ZeroMemory(szPath, sizeof(szPath));
	BROWSEINFO bi;
	strLocalDir.Format(_T("%s"), strLocalDir);
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = T2W((LPTSTR)strLocalDir.GetBuffer(NULL));

	bi.lpszTitle = _T("��ѡ��Ҫ���ļ������Ŀ¼��");
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);

	if(lp && SHGetPathFromIDList(lp, bi.pszDisplayName))
		return strLocalDir;
	else return NULL;

}

void CFTNNHTTPLogHelperDlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CFTNNHTTPLogHelperDlg::OnBnClickedDownload()
{
	CEdit * pEdit = (CEdit *)GetDlgItem(IDC_URLEDIT);

	CString URLStr ;//http://172.18.10.120:8080/HttpServer/VS2010SP1.iso
	pEdit->GetWindowTextW(URLStr);
	CString strServer;
	CString strFileName;
	CString strLocalFile = GetLocalFilePath();

	BOOL IsSuccess = HTTPGetFile(URLStr, strLocalFile);
	if (IsSuccess)
		MessageBox(_T("�ɹ�����"));
}


void CFTNNHTTPLogHelperDlg::OnEnChangeUrledit()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
