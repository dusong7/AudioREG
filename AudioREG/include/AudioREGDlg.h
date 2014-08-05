
// AudioREGDlg.h : ͷ�ļ�
//
#include "AudioRegcore.h"
#pragma once


// CAudioREGDlg �Ի���
class CAudioREGDlg : public CDialogEx
{
// ����
public:
	CAudioREGDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_AUDIOREG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	int audio2text(char *filename,char *outfile);
	int AudioReg_login();
	int ComputeSegNum(asseg_t *seg);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedStartButton();
	// �����ļ�·��
	CString filePath;
	// ʶ����չʾ
	CString resultShow;
	// ״̬����
	CString statusText;
	afx_msg void OnBnClickedExitButton();
//	afx_msg void OnEnChangeEdit();
	afx_msg void OnBnClickedFileButton();
};
