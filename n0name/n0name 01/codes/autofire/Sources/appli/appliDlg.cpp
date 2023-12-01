// appliDlg.cpp : implementation file
//

#include "stdafx.h"
#include "appli.h"
#include "appliDlg.h"

#include "driver_management.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAppliDlg dialog

CAppliDlg::CAppliDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAppliDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAppliDlg)
	m_emplacement = _T("");
	m_fullname = _T("");
	m_shortname = _T("");
	m_write = _T("");
	m_read = _T("");
	m_dos = _T("");
	m_ioctrl = _T("");
	m_iotype = -1;
	m_startupmode = -1;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_shortname =	(LPCSTR)"Exemple";
	m_fullname =	(LPCSTR)"Driver d'exemple";
	m_dos =			(LPCSTR)"\\\\.\\test_driver";
	m_emplacement =	(LPCSTR)"c:\\test_driver.sys";
	m_iotype = 0;
	m_startupmode = 0;
}

void CAppliDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAppliDlg)
	DDX_Text(pDX, IDC_EMPLACEMENT, m_emplacement);
	DDX_Text(pDX, IDC_FULLNAME, m_fullname);
	DDX_Text(pDX, IDC_SHORTNAME, m_shortname);
	DDX_Text(pDX, IDC_WRITE, m_write);
	DDX_Text(pDX, IDC_READ, m_read);
	DDX_Text(pDX, IDC_DOS, m_dos);
	DDX_Text(pDX, IDC_IOCTRL, m_ioctrl);
	DDX_CBIndex(pDX, IDC_COMBO1, m_iotype);
	DDX_CBIndex(pDX, IDC_COMBO2, m_startupmode);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAppliDlg, CDialog)
	//{{AFX_MSG_MAP(CAppliDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnEnregistrement)
	ON_WM_CREATE()
	ON_WM_CANCELMODE()
	ON_BN_CLICKED(IDC_BUTTON2, OnSuppression)
	ON_BN_CLICKED(IDC_BUTTON3, OnLancement)
	ON_BN_CLICKED(IDC_BUTTON4, OnArret)
	ON_BN_CLICKED(IDC_BUTTON5, OnWrite)
	ON_BN_CLICKED(IDC_BUTTON6, OnRead)
	ON_BN_CLICKED(IDC_BUTTON7, OnEnvoi)
	ON_BN_CLICKED(IDC_BUTTON8, OnStatus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAppliDlg message handlers

BOOL CAppliDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	//Initialise les champs

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAppliDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CAppliDlg::OnDestroy()
{
	WinHelp(0L, HELP_QUIT);
	CDialog::OnDestroy();
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAppliDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAppliDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

int CAppliDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	return 0;
}

void CAppliDlg::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}





void CAppliDlg::OnEnregistrement() 
{
	//Récupère les variables et les rend plus faciles à manipuler
	UpdateData(); 
	char* emplacement =(char*)LPCTSTR(m_emplacement); 
	char* nom_court =(char*)LPCTSTR(m_shortname); 
	char* nom_complet =(char*)LPCTSTR(m_fullname); 

	//Si il manque des infos, message et cassos
	if (!(strlen(emplacement) && strlen (nom_court) && strlen (nom_complet)))
	{
		MessageBox("Veuillez renseigner les champs ci dessus");
		return;
	}

	//Appel à la fonction et messages à afficher à l'utilisateur	
	switch (enregistre_driver(emplacement, nom_court, nom_complet, m_startupmode))
	{
		case DRIVER_MANAGER_OK:
			MessageBox("Driver enregistré avec succes");
			break;
		case DRIVER_MANAGER_ERROR_CREATE:
			MessageBox("Impossible d'enregistrer le driver, peut-être est il lancé");
			break;
		case DRIVER_MANAGER_ERROR_SCM:
			MessageBox("Accès au Service Control Manager interdit");
			break;
		case DRIVER_MANAGER_NO_FILE:
			MessageBox("Le fichier n'existe pas");
			break;
		case DRIVER_MANAGER_ERROR_ALREADY:
			MessageBox("Le driver existe déja");
			break;
	}
}


void CAppliDlg::OnSuppression() 
{
	//Récupère les variables et les rend plus faciles à manipuler
	UpdateData(); 
	char* emplacement =(char*)LPCTSTR(m_emplacement); 
	char* nom_court =(char*)LPCTSTR(m_shortname); 
	char* nom_complet =(char*)LPCTSTR(m_fullname); 

	//Si il manque des infos, message d'erreur et cassos
	if (!(strlen(emplacement) && strlen (nom_court) && strlen (nom_complet)))
	{
		MessageBox("Veuillez renseigner les champs ci dessus");
		return;
	}

	//Appel à la fonction et messages pour l'utilisateur
	switch (supprime_driver(nom_court))
	{
		case DRIVER_MANAGER_OK:
			MessageBox("Driver supprimé avec succes");
			break;
		case DRIVER_MANAGER_ERROR_ACCESS:
			MessageBox("Impossible d'accéder au driver, il n'est peut-être pas enregistré");
			break;
		case DRIVER_MANAGER_ERROR_SCM:
			MessageBox("Accès au Service Control Manager interdit");
			break;
		case DRIVER_MANAGER_ERROR_DELETE:
			MessageBox("Impossible de supprimer le driver");
			break;
	}
}


void CAppliDlg::OnLancement() 
{
	//Récupère les variables et les rend plus faciles à utiliser
	UpdateData(); 
	char* emplacement =(char*)LPCTSTR(m_emplacement); 
	char* nom_court =(char*)LPCTSTR(m_shortname); 
	char* nom_complet =(char*)LPCTSTR(m_fullname); 

	//Si il manque des infos, message et cassos
	if (!(strlen(emplacement) && strlen (nom_court) && strlen (nom_complet)))
	{
		MessageBox("Veuillez renseigner les champs ci dessus");
		return;
	}

	//Appel à la fonction et messages utilisateur
	switch (load_driver(nom_court))
	{
		case DRIVER_MANAGER_OK:
			MessageBox("Driver démarré avec succes");
			break;

		case DRIVER_MANAGER_ERROR_ACCESS:
			MessageBox("Impossible d'accéder au driver, il n'est peut-être pas enregistré");
			break;

		case DRIVER_MANAGER_ERROR_SCM:
			MessageBox("Accès au Service Control Manager interdit");
			break;

		case DRIVER_MANAGER_ERROR_RUN:
			MessageBox("Impossible de démarrer le driver, il est peut-être déja lancé");
			break;
	}	
}


void CAppliDlg::OnArret() 
{
	//Récupération des variables et les rend plus faciles à manipuler
	UpdateData(); 
	char* emplacement =(char*)LPCTSTR(m_emplacement); 
	char* nom_court =(char*)LPCTSTR(m_shortname); 
	char* nom_complet =(char*)LPCTSTR(m_fullname); 

	//Si il manque des informations, message et cassos
	if (!(strlen(emplacement) && strlen (nom_court) && strlen (nom_complet)))
	{
		MessageBox("Veuillez renseigner les champs ci dessus");
		return;
	}

	//Appel à la fonction et message utilisateur	
	switch (unload_driver(nom_court))
	{
		case DRIVER_MANAGER_OK:
			MessageBox("Driver arrêté avec succes");
			break;
		case DRIVER_MANAGER_ERROR_ACCESS:
			MessageBox("Impossible d'accéder au driver, il n'est peut-être pas enregistré");
			break;
		case DRIVER_MANAGER_ERROR_SCM:
			MessageBox("Accès au Service Control Manager interdit");
			break;
		case DRIVER_MANAGER_ERROR_SHUTDOWN:
			MessageBox("Impossible d'éteindre le driver, il n'est peut-être pas lancé");
			break;
	}	
}


void CAppliDlg::OnWrite() 
{
	// Récupère les champs
	UpdateData(); 
	if (!ecris_driver ((char*)LPCTSTR(m_dos), (char*)LPCTSTR(m_write) ))
		MessageBox("Ecriture impossible, le driver ne semble pas chargé");
}


void CAppliDlg::OnRead() 
{
	//Le buffer local pour stoquer les infos lues
	char buffer_lecture[1024];
	unsigned long taille_buffer_lecture = 1024;
	
	//Récupère les variables
	UpdateData(); 
	
	//Appelle la fonction de lecture
	if (!lis_driver ((char*)LPCTSTR(m_dos), buffer_lecture, &taille_buffer_lecture))
		MessageBox("Lecture impossible, le driver ne semble pas chargé");
	else
	{
		//Et renvoie la valeur dans le champ
		m_read = (LPCTSTR)buffer_lecture;
 		UpdateData(FALSE);
	}
}


void CAppliDlg::OnEnvoi() 
{
	//Buffer local pour la partie MDL de la communication avec le driver	
	char buffer_ioctrl[1024];
	unsigned long taille_buffer_ioctrl = 1024;

	//Les deux types de communication possibles
	DWORD IOCTL_EXAMPLE_SAMPLE_DIRECT_IN_IO = 2285569;
	DWORD IOCTL_EXAMPLE_SAMPLE_DIRECT_OUT_IO = 2285574;

	//Récupération des variables
	UpdateData(); 

	//Un mode de communication doit etre sélectionné, sinon cassos
	CString string_controle;
	int nIndex = GetDlgItemText(IDC_COMBO1, string_controle); 

	//Action
	bool verdict;
	if (m_iotype == 0)
		verdict = ioctrl_driver ((char*)LPCTSTR(m_dos), (char*)LPCTSTR(m_ioctrl), IOCTL_EXAMPLE_SAMPLE_DIRECT_IN_IO, buffer_ioctrl, &taille_buffer_ioctrl );
	else
		verdict = ioctrl_driver ((char*)LPCTSTR(m_dos), (char*)LPCTSTR(m_ioctrl), IOCTL_EXAMPLE_SAMPLE_DIRECT_OUT_IO, buffer_ioctrl, &taille_buffer_ioctrl);

	if (verdict)
	{
		m_ioctrl = (LPCTSTR)buffer_ioctrl;
		UpdateData(FALSE); 
	}
	else
		MessageBox("IO-Control impossible, le driver ne semble pas chargé");
}


void CAppliDlg::OnStatus() 
{
	// Récupère les variables
	UpdateData(); 

	switch (verifie_status((char*)LPCTSTR(m_shortname)))
	{
	case DRIVER_MANAGER_STATUS_ABSENT:
		MessageBox("Le driver n'est pas enregistré");
		break;
	case DRIVER_MANAGER_STATUS_ACTIF:
		MessageBox("Le driver est enregistré et actif");
		break;
	case DRIVER_MANAGER_STATUS_INACTIF:
		MessageBox("Le driver est enregistré mais inactif");
		break;
	}
	
}
