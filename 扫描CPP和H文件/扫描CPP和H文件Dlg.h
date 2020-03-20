


#pragma once
#include "afxwin.h"
#include "afxcmn.h"



class C扫描CPP和H文件Dlg : public CDialogEx
{

public:
	C扫描CPP和H文件Dlg(CWnd* pParent = NULL);	


#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CPPH_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	



protected:
	HICON m_hIcon;


	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	
	CEdit Path_root;
	afx_msg void scanfolders();
	CTreeCtrl file_tree;
	CStatic status_now;
	CListCtrl file_list;
	CButton btn_start;
};
