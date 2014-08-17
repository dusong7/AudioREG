
// AudioREGDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AudioREG.h"
#include "AudioREGDlg.h"
#include "afxdialogex.h"
#include "AudioRegcore.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CAudioREGDlg 对话框




CAudioREGDlg::CAudioREGDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAudioREGDlg::IDD, pParent)
	, filePath(_T(""))
	, resultShow(_T(""))
	, statusText(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAudioREGDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, File_Edit, filePath);
	DDX_Text(pDX, Result_Edit, resultShow);
	DDX_Text(pDX, regStatus, statusText);
}

BEGIN_MESSAGE_MAP(CAudioREGDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(Start_Button, &CAudioREGDlg::OnBnClickedStartButton)
	ON_BN_CLICKED(Exit_Button, &CAudioREGDlg::OnBnClickedExitButton)
//	ON_EN_CHANGE(Result_Edit, &CAudioREGDlg::OnEnChangeEdit)
ON_BN_CLICKED(Choose_Button, &CAudioREGDlg::OnBnClickedFileButton)
ON_EN_CHANGE(Result_Edit, &CAudioREGDlg::OnEnChangeEdit)
END_MESSAGE_MAP()


// CAudioREGDlg 消息处理程序

BOOL CAudioREGDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CAudioREGDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAudioREGDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CAudioREGDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




UINT CAudioREGDlg::MyThreadFunction(LPVOID pParam)
{
	CAudioREGDlg *p = (CAudioREGDlg*)pParam;
	char *infilename = NULL;
	USES_CONVERSION;
	infilename = T2A(p->filePath);
	char outfilename[10] = "temp.txt";
	int ret = p->AudioReg_login();
	if ( ret != MSP_SUCCESS )
	{
		printf("MSPLogin failed , Error code %d.\n",ret);
		//return -1;
	}
	int Finish = p->audio2text(infilename,outfilename);
	if (Finish == 0)
	{
		p->statusText = "识别已完成";	
		p->GetDlgItem(regStatus)->SetWindowTextW(p->statusText);

		p->GetDlgItem(Start_Button)->EnableWindow(TRUE);
	}
	MSPLogout();//退出登录
	return 0;

}




void CAudioREGDlg::OnBnClickedStartButton()
{
	// TODO: 在此添加控件通知处理程序代码

	
	resultShow = "hello";
	statusText = "识别已启动";	
	GetDlgItem(regStatus)->SetWindowTextW(statusText);
	GetDlgItem(Start_Button)->EnableWindow(FALSE);
	
	GetDlgItem(Result_Edit)->SetWindowTextW(resultShow);
	GetDlgItem(Result_Edit)->UpdateWindow();
	AfxBeginThread(MyThreadFunction,this);
	/*
	char *infilename = NULL;
	USES_CONVERSION;
	infilename = T2A(filePath);
	char outfilename[10] = "temp.txt";
	int ret = AudioReg_login();
	if ( ret != MSP_SUCCESS )
	{
		printf("MSPLogin failed , Error code %d.\n",ret);
		//return -1;
	}
	int Finish = audio2text(infilename,outfilename);
	if (Finish == 0)
	{
		statusText = "识别已完成";	
		GetDlgItem(regStatus)->SetWindowTextW(statusText);

		GetDlgItem(Start_Button)->EnableWindow(TRUE);
	}

	MSPLogout();//退出登录
	*/

}


void CAudioREGDlg::OnBnClickedExitButton()
{
	// TODO: 在此添加控件通知处理程序代码
	
	EndDialog(NULL);
}

int CAudioREGDlg::audio2text(char *infilename,char *outfilename)
{

	char logStr[1000];                     /* log buffer                         */
	sigstream_t *s;			           /* input signal stream                 */
	asseg_t *seg = NULL;
	asseg_t *segTemp = NULL;
	int format = SPRO_SIG_PCM16_FORMAT; /* signal file format                    */
	float Fs = 16000.0;                /* input signal sample rate               */
	int swap = 0;                     /* change input sample byte order          */
	size_t ibs = 10000000;           /* input buffer size                        */
	string ss = "world";
	string *rStr = &ss;
	clock_t start,finish;
	CString newStr;
	head_pama header;
	header=wav_header_read(infilename);

	if(header.bits != 16)
	{
		//printf("请输入精度为16bits的音频!\n");
		LogWrite("请输入精度为16bits的音频!", 1, 0);
		return 1;
	}

	if(header.channels != 1)
	{
		//printf("请输入单声道音频!\n");
		LogWrite("请输入单声道音频!", 1, 0);
		return 1;
	}

	if(header.rate != 16000)
	{
		//printf("请输入采样率为16k的音频!\n");
		LogWrite("请输入采样率为16k的音频!", 1, 0);
		return 1;
	}

	sprintf(logStr, "成功打开待转译音频%s!", infilename);
	LogWrite(logStr, 0, 0);


	sprintf(logStr, "成功打开待转译音频%s!", infilename);
	LogWrite(logStr, 0, 0);

	const int datasize = header.datasize;
	float temptime= (float)datasize/header.rate;
	const float datatime=temptime/2;

	if((s = sig_stream_open(infilename, format, Fs, ibs, swap)) == NULL)
	{
		//fprintf(stderr, "ssad error -- cannot open input signal stream %s\n", (mWAV) ? (mWAV) : "stdin");
		sprintf(logStr, "ssad error -- cannot open input signal stream %s！", (infilename) ? (infilename) : "stdin");
		LogWrite(logStr, 1, 1);
		return(1);
	}

	if((seg = silence_detection(s)) == NULL)
	{
		LogWrite("Speech Activity Detection Error！", 1, 1);
		return(1);
	}

	segTemp = seg;
	const int nSeg = ComputeSegNum(seg);
	sprintf(logStr, "%s静音检测成功，共包含%d个音频段!",infilename, nSeg);
	LogWrite(logStr, 0, 1);

	float start_time = 0.0, end_time = 0.0, newstart_time = 0.0, newend_time = 0.0;
	int i = 0,current = 1;



	LogWrite("开始对每个分割后的音频段进行语音转录!", 0, 2);



	if(outfilename != NULL)
	{
		FILE* out;
		out = fopen(outfilename, "wb+");

		if(out == NULL)
		{
			sprintf(logStr, "Can't open output transcript file %s!", outfilename);
			LogWrite(logStr, 1, 0);
			return(1);
		}
		LogWrite("输出文件已经正常打开",0,0);
		while(segTemp)
		{
			start_time = get_seg_start_time(segTemp);
			end_time = get_seg_end_time(segTemp);

			if(start_time>0.5)
				newstart_time = start_time-0.5;
			else 
				newstart_time = start_time;
			if((end_time+0.5)<datatime)
				newend_time =end_time+0.5;
			else 
				newend_time = end_time;
			rStr->clear();
			start = clock();


			int transResult = seg_recognizer(infilename, newstart_time, newend_time, rStr);
			finish = clock();

			sprintf(logStr, "第%d个片段所花时间%f second\n",current, (double)(finish - start) / CLOCKS_PER_SEC);
			LogWrite(logStr, 3, 1);
			if(transResult != 0)
			{
				sprintf(logStr, "识别程序出现问题，返回错误码为%d",transResult);
				LogWrite(logStr, 1, 2);
				sig_stream_close(s);
				seg_list_free(&seg);

				rStr->clear();
				fclose(out);
				return -1;
			}

			if(fprintf(out, "%.2f %.2f %s\n", start_time,end_time, rStr->c_str()) < 0)
			{
				sprintf(logStr,"%d个片段写入识别结果出错",i);
				LogWrite(logStr,1,0);
			}

			fflush(out);
			//out<<mText[i].startTime<<" "<<mText[i].endTime<<" "<<mText[i].textStr<<endl;
			newStr = rStr->c_str();
			resultShow = resultShow+(CString)("\r\n") + newStr;
			GetDlgItem(Result_Edit)->SendMessage(WM_VSCROLL,Start_Button);
			GetDlgItem(Result_Edit)->SetWindowTextW(resultShow);
			GetDlgItem(Result_Edit)->UpdateWindow();


			i++;
			current++;
			segTemp = segTemp->next;
			Sleep(10);
		}

		sprintf(logStr, "%s翻译完毕\n", infilename);
		LogWrite(logStr, 0, 2);
		/* ----- clean memory ----- */
		sig_stream_close(s);
		seg_list_free(&seg);

		rStr->clear();
		fclose(out);

		}

	return 0;
	
}


int CAudioREGDlg::ComputeSegNum(asseg_t *seg)
{		         
	const asseg_t *p = seg;
	int nSeg = 0;
	while(p)
	{
		nSeg++;
		p = p->next;
	}

	return nSeg;
}


int CAudioREGDlg::AudioReg_login()
{
	int ret = 0;
	char *params = "lang=sms,acous=anhui,rate=16k,aue=raw,nbest=3,auf=audio/L16;rate=16000";
	const char* login_configs = "appid = 53d5d940, timeout = 30000, work_dir =   .  ";//登录参数
	//const char* login_configs = "appid = 53d78317, work_dir =   .  ";//登录参数
	//const char *session_id = NULL;
	//int errorCode = 0;
	//用户登录
	char username[20] = "poryfly";
	ret = MSPLogin(username,NULL, login_configs);//第一个参数为用户名，第二个参数为密码，第三个参数是登录参数，用户名和密码需要在http://open.voicecloud.cn注册并获取appid

	return ret;
}

//void CAudioREGDlg::OnEnChangeEdit()
//{
//	// TODO:  如果该控件是 RICHEDIT 控件，它将不
//	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
//	// 函数并调用 CRichEditCtrl().SetEventMask()，
//	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
//
//	// TODO:  在此添加控件通知处理程序代码
//	UpdateData(FALSE);
//	//SetDlgItemText()
//}


void CAudioREGDlg::OnBnClickedFileButton()
{
	// TODO: 在此添加控件通知处理程序代码
	statusText = "识别未启动";
	GetDlgItem(regStatus)->SetWindowTextW(statusText);
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,(LPCTSTR)_TEXT("wav files(*.wav)|*.wav"),NULL);
	if (dlg.DoModal() == IDOK)
	{
		filePath = dlg.GetPathName();
		CString filename = filePath.Mid(filePath.ReverseFind('\\')+1);
		GetDlgItem(File_Edit)->SetWindowTextW(filename);
	}

	else
	{
		return;
	}

}


void CAudioREGDlg::OnEnChangeEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
