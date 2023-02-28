// RankDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Project.h"
#include "RankDlg.h"
#include "afxdialogex.h"


// RankDlg �Ի���

IMPLEMENT_DYNAMIC(CRankDlg, CDialogEx)

CRankDlg::CRankDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_RANK_DIALOG, pParent)
{

}

CRankDlg::~CRankDlg()
{
}

void CRankDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRankDlg, CDialogEx)
	ON_WM_PAINT()
	ON_MESSAGE(WM_RANK_MSG, &CRankDlg::OnRankMsg)
	ON_WM_CLOSE()
	ON_STN_CLICKED(IDOK, &CRankDlg::OnStnClickedOk)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// RankDlg ��Ϣ�������
afx_msg LRESULT CRankDlg::OnRankMsg(WPARAM wParam, LPARAM lParam)
{

	int* m_DataReciever = (int *)wParam;
	CString buf;
	buf.Format("%d", m_DataReciever[0]);
	GetDlgItem(IDC_STATIC_HISTORY_HIGHEST_SCORE)->SetWindowText(buf);
	buf.Format("%d", m_DataReciever[1]);
	GetDlgItem(IDC_STATIC_SCORE)->SetWindowText(buf);
	buf.Format("%d", m_DataReciever[2]);
	GetDlgItem(IDC_STATIC_RANK)->SetWindowText(buf);
	return 0;
}

BOOL CRankDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//ShowWindow(SW_MAXIMIZE);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	// Get the client 
	GetClientRect(&m_client);
	// Load the background Picture
	m_RankDlgBackground.Load("res\\SelectDlgBackGround.png");
	m_MoveRankDlg();
	//��ȡ������¼����ʾ
	ReadAndSetRank();
	SetFocus();
	return TRUE;
	// return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CRankDlg::OnPaint()
{
	CPaintDC dc(this);
	// device context for painting
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	// Set the Initialize value
	m_DrawRankDlgBackGround();
}

void CRankDlg::m_DrawRankDlgBackGround()
{
	CDC *cDC = this->GetDC();
	m_RankDlgBackground.Draw(cDC->m_hDC, m_client);
}

void CRankDlg::m_MoveRankDlg()
{
	CRect position;
	
	position.left = (int)(m_client.right * 0.10);
	position.right = (int)(m_client.right * 0.60);

	position.top = (int)(m_client.bottom*0.10);
	position.bottom = (int)(m_client.bottom * 0.30);
	GetDlgItem(IDC_STATIC_LABEL1)->MoveWindow(position);

	position.top = (int)(m_client.bottom*0.35);
	position.bottom = (int)(m_client.bottom * 0.55);
	GetDlgItem(IDC_STATIC_LABEL2)->MoveWindow(position);

	position.top = (int)(m_client.bottom*0.60);
	position.bottom = (int)(m_client.bottom * 0.80);
	GetDlgItem(IDC_STATIC_LABEL3)->MoveWindow(position);



	position.left = (int)(m_client.right * 0.60);
	position.right = (int)(m_client.right * 0.90);

	position.top = (int)(m_client.bottom*0.07);
	position.bottom = (int)(m_client.bottom * 0.27);
	GetDlgItem(IDC_STATIC_HISTORY_HIGHEST_SCORE)->MoveWindow(position);

	position.top = (int)(m_client.bottom*0.32);
	position.bottom = (int)(m_client.bottom * 0.52);
	GetDlgItem(IDC_STATIC_SCORE)->MoveWindow(position);

	position.top = (int)(m_client.bottom*0.57);
	position.bottom = (int)(m_client.bottom * 0.77);
	GetDlgItem(IDC_STATIC_RANK)->MoveWindow(position);


	position.left = (int)(m_client.right * 0.80);
	position.right = (int)(m_client.right * 0.99);
	position.top = (int)(m_client.bottom*0.85);
	position.bottom = (int)(m_client.bottom * 0.95);
	GetDlgItem(IDOK)->MoveWindow(position);
}

void  CRankDlg::ReadAndSetRank(){
	CString buff;
	int temp;
	FILE *m_CurrentRankFile;
	// open Files
	fopen_s(&m_CurrentRankFile, "res\\CurrentRank.temp", "r");
	fscanf_s(m_CurrentRankFile, "%d", &temp);

	// Reset the Rank Static default Value
	buff.Format("%d������", RECORD_NUMBER);
	GetDlgItem(IDC_STATIC_RANK)->SetWindowTextA(buff);

	buff.Format("%d", temp);
	GetDlgItem(IDC_STATIC_HISTORY_HIGHEST_SCORE)->SetWindowTextA(buff);
	fscanf_s(m_CurrentRankFile, "%d", &temp);
	buff.Format("%d", temp);
	GetDlgItem(IDC_STATIC_SCORE)->SetWindowTextA(buff);
	fscanf_s(m_CurrentRankFile, "%d", &temp);

	// Check whether the player's rank is out of RECORD_NUMBER
	if (temp >= RECORD_NUMBER)return;

	buff.Format("%d", temp);
	GetDlgItem(IDC_STATIC_RANK)->SetWindowTextA(buff);
	fclose(m_CurrentRankFile);
}

void CRankDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	this->DestroyWindow();
	CDialogEx::OnClose();
}

void CRankDlg::OnStnClickedOk()
{
	CDialogEx::OnCancel();
}

HBRUSH CRankDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if ((pWnd->GetDlgCtrlID() == IDOK) || (pWnd->GetDlgCtrlID() == IDC_STATIC_LABEL1) ||
		(pWnd->GetDlgCtrlID() == IDC_STATIC_LABEL2) || (pWnd->GetDlgCtrlID() == IDC_STATIC_LABEL3))
	{
		pDC->SetBkMode(TRANSPARENT);
		//pDC->SetBkColor(RGB(144, 238, 144));
		//pDC->SetTextColor(RGB(144, 238, 144));
		CFont *m_Font = new CFont();
		pDC->SetTextColor(RGB(0, 0, 0));

		m_Font->CreatePointFont(240, "�����п�", NULL);
		pDC->SelectObject(m_Font);

		delete m_Font;

		return   (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	if ((pWnd->GetDlgCtrlID() == IDC_STATIC_HISTORY_HIGHEST_SCORE) || (pWnd->GetDlgCtrlID() == IDC_STATIC_SCORE) ||
		(pWnd->GetDlgCtrlID() == IDC_STATIC_RANK))
	{
		//����������
		CFont *font = new CFont();
		pDC->SetBkMode(TRANSPARENT);
		//pDC->SetBkColor(RGB(135, 206, 235));
		pDC->SetTextColor(RGB(218, 165, 32));

		font->CreatePointFont(360, "����", NULL);
		pDC->SelectObject(font);

		delete font;

		return   (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
