
#include "stdafx.h"
#include "扫描CPP和H文件.h"
#include "扫描CPP和H文件Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

C扫描CPP和H文件Dlg::C扫描CPP和H文件Dlg(CWnd* pParent )
	: CDialogEx(IDD_CPPH_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C扫描CPP和H文件Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, Edit_path, Path_root);
	DDX_Control(pDX, File_tree, file_tree);
	DDX_Control(pDX, static_status, status_now);
	DDX_Control(pDX, File_list, file_list);
	DDX_Control(pDX, IDC_BUTTON1, btn_start);
}

BEGIN_MESSAGE_MAP(C扫描CPP和H文件Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &C扫描CPP和H文件Dlg::scanfolders)
END_MESSAGE_MAP()

void C扫描CPP和H文件Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}
HCURSOR C扫描CPP和H文件Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/*从这里往下看，系统自动注释的均已删除，凡绿色注释均为天杰所写(*^∇^*)*/
C扫描CPP和H文件Dlg *pt_wincla = NULL;
BOOL C扫描CPP和H文件Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);		

	/*初始化代码。向列表控件插入标头，并把this指针传给全局指针变量pt_wincla，
		通过该变量实现操作Dlg类成员函数
	*/
	CString listtitle[] = { _T("文件名"),_T("行数"),_T("路径") };
	for (int i = 0; i < 2; i++)
	{
		file_list.InsertColumn(i, listtitle[i], LVCFMT_CENTER, 88);
	}
	file_list.InsertColumn(2, listtitle[2], LVCFMT_CENTER, 200);
	pt_wincla = this;
	Path_root.SetWindowTextW(_T("支持拖放输入"));
	return TRUE;  
}

	/****拖放功能函数****/
void C扫描CPP和H文件Dlg::OnDropFiles(HDROP hDropInfo)
{
	/*通过窗口自带OnDropFiles事件，实现拖放读取文件功能，
		并根据文件路径判断是否为目录，更新到控件中*/
	wchar_t filepath[MAX_PATH] = { 0 };   
	DragQueryFile(hDropInfo, 0,filepath, MAX_PATH);	
	if (GetFileAttributes(filepath) == 0x10 || GetFileAttributes(filepath) == 0x11)
	{
		Path_root.SetWindowText(filepath);
	}
	else
	{
		MessageBox(_T("请拖入文件夹(∩_∩）"));
	}
	CDialogEx::OnDropFiles(hDropInfo);
}

/*定义用于线程函数传参的结构体*/
typedef struct canshu_stru
{
	CString path_folder;	//遍历目录
	HTREEITEM  deepth;	//树形控件的项目值
}*canshu_pt;

	/*全局变量*/
long file_num = 0;//CPP文件和H文件数量
long file_line = 0;//CPP文件和H中文本数量
BOOL status_find = FALSE; //当前是否正在查找

//扫描文件夹按钮点击事件
/*1、判断路径是否为目录，并开始创建新线程遍历目录中的文件
2、测试过程中发现有些目录属性为只读，返回代码为0x10和0x01的与的结果（FILE_ATTRIBUTE_DIRECTORY为0x10,FILE_ATTRIBUTE_READONLY为0x01），故条件判断目录为目录或者只读目录均可。
3、当直接在按钮1单击事件中进行遍历操作，遍历目录过大时会影响窗口控件的更新，故创建新线程来遍历
*/
void C扫描CPP和H文件Dlg::scanfolders()
{
	/*判断按钮当前状态*/
	if (status_find == FALSE)
	{
		status_find = TRUE;
		btn_start.SetWindowTextW(_T("停止"));
	} 
	else
	{
		status_find = FALSE;
		btn_start.SetWindowTextW(_T("遍历文件夹"));
	}

	/*开始遍历啦*/
	if (status_find == TRUE)
	{
		//获取文本框中的路径
		CString path;
		Path_root.GetWindowTextW(path);

		if (GetFileAttributes(path) == 0x10 || GetFileAttributes(path) == 0x11)//判断路径是否为目录或者只读目录
		{
				
			HANDLE Hthread = NULL;
			DWORD WINAPI file_traverse(LPVOID lpParamter);//函数体在下方，故声明一下线程函数
			canshu_pt para_pt = new canshu_stru;
			para_pt->path_folder = path;
			para_pt->deepth = NULL;
			
			try
			{
				//开始遍历，判断线程是否创建成功，错误则并返回错误代码
				file_num = 0;
				file_line = 0;
				file_list.DeleteAllItems();
				file_tree.DeleteAllItems();
				Hthread = CreateThread(NULL, 0, file_traverse, para_pt, 0, NULL);
				if (Hthread == NULL)
				{
					throw GetLastError();
				}
				CloseHandle(Hthread);
			}
			catch (DWORD e)
			{
				LPVOID lp_msg;
				FormatMessageW(
					FORMAT_MESSAGE_ALLOCATE_BUFFER |
					FORMAT_MESSAGE_FROM_SYSTEM |
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					e,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					(LPWSTR)&lp_msg,
					0,
					NULL
				);
				//提示错误信息
				MessageBox((LPWSTR)lp_msg);
				LocalFree(lp_msg);
			}
		}
		else
		{
			MessageBox(_T("该路径不是目录或者目录无效！"));

		}
	}
}

		/*遍历功能*/
DWORD WINAPI file_traverse(LPVOID lpParamter) {
	/*处理传入的参数*/
	canshu_pt para_pt = (canshu_pt)lpParamter;
	CString file_path = para_pt->path_folder;
	file_path += _T("\\");
	file_path += _T("*.*");
	CFileFind file_finded;
	BOOL file_result =  file_finded.FindFile(file_path);
	while (file_result)
	{
		if (status_find==FALSE)
		{
			break;
		}
		file_result = file_finded.FindNextFileW();
		if (!file_finded.IsDots())	
		{
			/*判断该文件是否为自身目录或者上层目录，然后读取文件名字和路径，并写入树形控件
			，因为此函数为线程函数，故非类成员函数，所以在操作控件时通过Dlg类指针来操作成员函数
			*/
			CString file_name     =    file_finded.GetFileName();
			CString file_path     =    file_finded.GetFilePath();
			HTREEITEM item_parent =    pt_wincla->file_tree.InsertItem(file_name, para_pt->deepth);
			CString file_ext      =    PathFindExtensionW(file_name);
			pt_wincla->status_now.SetWindowTextW(file_path);

			/*获取文件扩展名与.CPP和.H进行比较，属于两者之一则读取文件并返回文本行数写入列表控件*/
			if (file_ext == _T(".cpp") || file_ext == _T(".h"))
			{
				long file_read(CString path_file);
				long line = file_read(file_path);
				file_line += line;
				file_num++;
				CString cstr_line;
				cstr_line.Format(_T("%ld"),line);
				pt_wincla->file_list.InsertItem(0, file_name);
				pt_wincla->file_list.SetItemText(0, 1, cstr_line);
				pt_wincla->file_list.SetItemText(0, 2, file_path);
			}
			if (file_finded.IsDirectory())
			{
				/*判断如果该文件为目录，则递归遍历该目录*/
				canshu_pt para_child    =   new canshu_stru;
				para_child->deepth      =   item_parent;
				para_child->path_folder =   file_path;
				file_traverse(para_child);
			}
		}
		
	}
	/*判断是否为递归中的函数，输出结果*/
	if ( para_pt->deepth == NULL)
	{
		file_finded.Close();
		status_find = FALSE;
		pt_wincla->btn_start.SetWindowTextW(_T("遍历文件夹"));
		CString cstr_file_num, cstr_file_line;
		cstr_file_num.Format(_T("%ld"), file_num);
		cstr_file_line.Format(_T("%ld"), file_line);
		pt_wincla->status_now.SetWindowText(_T("遍历完毕，源文件和头文件共有:   ") + cstr_file_num + _T("个， 总行数为：  ") + cstr_file_line);
		delete para_pt;
	}

	return 0;
}

long file_read(CString path_file) {
	long line = 0;
	/*按行读取文件，存入contents变量并返回行数line
	,测试过程中发现会遇到文件占用无法读取的情况，因此加入错误处理，
	并返回错误信息。
	*/
	
	CString content_file;
	CStdioFile R_file;
	try
	{
		if(!R_file.Open(path_file, CFile::modeRead)){
			throw GetLastError();
		}
		
		while (R_file.ReadString(content_file))
		{
			if (content_file.GetLength() != 0)
			{
				line++;
			}
		}
		R_file.Close();
	}
	catch (DWORD e)
	{
		/*捕捉错误并提示错误信息*/
		LPVOID lp_msg;
		FormatMessageW(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			e,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lp_msg,
			0,
			NULL
		);
		MessageBox(NULL,path_file+_T("文件读取错误，错误原因：\n")+(LPWSTR)lp_msg , _T("Error"), MB_OK | MB_ICONINFORMATION);
		LocalFree(lp_msg);
	}
	return line;
}