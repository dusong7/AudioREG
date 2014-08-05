
// AudioREGDlg.h : 头文件
//
#include "AudioRegcore.h"
#pragma once


// CAudioREGDlg 对话框
class CAudioREGDlg : public CDialogEx
{
// 构造
public:
	CAudioREGDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_AUDIOREG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	int audio2text(char *filename,char *outfile);
	int AudioReg_login();
	int ComputeSegNum(asseg_t *seg);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedStartButton();
	// 输入文件路径
	CString filePath;
	// 识别结果展示
	CString resultShow;
	// 状态变量
	CString statusText;
	afx_msg void OnBnClickedExitButton();
//	afx_msg void OnEnChangeEdit();
	afx_msg void OnBnClickedFileButton();
};
