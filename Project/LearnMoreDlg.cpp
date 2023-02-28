// LearnMoreDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Project.h"
#include "LearnMoreDlg.h"
#include "afxdialogex.h"


// CLearnMoreDlg �Ի���

IMPLEMENT_DYNAMIC(CLearnMoreDlg, CDialogEx)

CLearnMoreDlg::CLearnMoreDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LEARNMORE_DIALOG, pParent)
{

}

CLearnMoreDlg::~CLearnMoreDlg()
{

}

void CLearnMoreDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MEDIA_PLAYER, m_MediaPlayer);
	DDX_Control(pDX, IDC_INTRODUCTION1, m_LearnmoreIntroduction1);
	DDX_Control(pDX, IDC_INTRODUCTION2, m_LearnmoreIntroduction2);
}


BEGIN_MESSAGE_MAP(CLearnMoreDlg, CDialogEx)
	//ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CLearnMoreDlg ��Ϣ�������


BOOL CLearnMoreDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//GetClientRect(&m_client);
	//// Load the background Picture
	//m_learnMoreDlgBackground.Load("res\\SelectDlgBackGround.png");

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_MediaPlayer.put_URL("res\\alpha1.wmv");
	// Set Font
	m_Font.CreatePointFont(150, "Arial", NULL);
	m_LearnmoreIntroduction1.SetFont(&m_Font, true);
	m_LearnmoreIntroduction2.SetFont(&m_Font, true);

	//SetTimer(TIMER_REPLAY, REPLAY_CHECK_TIME, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

//void CLearnMoreDlg::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//					   // TODO: �ڴ˴������Ϣ����������
//					   // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
//					   // Set the Initialize value
//	m_DrawLearnMoreDlgBackGround();
//}

//void CLearnMoreDlg::m_DrawLearnMoreDlgBackGround()
//{
//	CDC *cDC = this->GetDC();
//	m_learnMoreDlgBackground.Draw(cDC->m_hDC, m_client);
//}



