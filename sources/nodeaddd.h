//
// FILE: nodeaddd.h -- Node Add Dialog.  Handles creation of new nodes for the
//                      extensive form display.
//
// $Id$
//

typedef enum {NodeAddNew,NodeAddIset} NodeAddMode;
class NodeAddDialog : public MyDialogBox
{
private:
	BaseEfg &ef;
	int		branches;
	char	*player_name;
	char	*iset_name;
	Bool	set_names;
	// keep track of the panel items
	wxChoice	*player_item;
	wxChoice	*iset_item;
	wxText		*branch_item;
	// event handlers-low level
	static void player_func(wxChoice &ob,wxCommandEvent &ev);
	static void iset_func(wxChoice &ob,wxCommandEvent &ev);
	static void branch_func(wxText &ob,wxCommandEvent &ev);
	// event handlers-high level
	void OnPlayer(const char *name);
	void OnIset(const char *name);
	void OnBranch(void);
public:
	// Constructor
	NodeAddDialog(BaseEfg &ef_,EFPlayer *player=0,Infoset *infoset=0,int branches=2,wxFrame *frame=0);
	// Destructor
	~NodeAddDialog(void);
	// DataAccess
	NodeAddMode	GetAddMode(void) const;
	EFPlayer 			*GetPlayer(void);
	Infoset 		*GetInfoset(void);
	int					GetBranches(void);
	Bool				SetNames(void) const;
};

//************************************** CONSTRUCTOR **************************
NodeAddDialog::NodeAddDialog(BaseEfg &ef_,EFPlayer *player,Infoset *infoset,int branches_,wxFrame *frame)
						:MyDialogBox(frame,"Add Node",EFG_NODE_HELP),ef(ef_),branches(branches_)
{
set_names=TRUE;
wxStringList *player_list=new wxStringList;player_name=new char[20];
player_list->Add("Chance");
for (int i=1;i<=ef.NumPlayers();i++) player_list->Add((ef.Players()[i])->GetName());
player_list->Add("New Player");
if (player) strcpy(player_name,player->GetName()); else strcpy(player_name,"New Player");
wxFormItem *player_fitem=Add(wxMakeFormString("Player", &player_name, wxFORM_CHOICE,
			new wxList(wxMakeConstraintStrings(player_list),0)));
Add(wxMakeFormNewLine());
wxFormItem *branch_fitem=Add(wxMakeFormShort("Branches",&branches, wxFORM_TEXT));
wxStringList *infoset_list=new wxStringList;iset_name=new char[20];
infoset_list->Add("New Infoset");
if (infoset) strcpy(iset_name,infoset->GetName()); else strcpy(iset_name,"New");
wxFormItem *iset_fitem=Add(wxMakeFormString("Iset", &iset_name, wxFORM_CHOICE,
			new wxList(wxMakeConstraintStrings(infoset_list),0)));
Add(wxMakeFormNewLine());
Add(wxMakeFormBool("Set names",&set_names));
AssociatePanel();
player_item=(wxChoice *)player_fitem->GetPanelItem();
player_item->Callback((wxFunction)player_func);
player_item->SetClientData((char *)this);
iset_item=(wxChoice *)iset_fitem->GetPanelItem();
iset_item->Callback((wxFunction)iset_func);
iset_item->SetClientData((char *)this);
iset_item->SetStringSelection("New");
branch_item=(wxText *)branch_fitem->GetPanelItem();
branch_item->Callback((wxFunction)branch_func);
branch_item->SetClientData((char *)this);
// Fake a player selection
wxCommandEvent ev(wxEVENT_TYPE_CHOICE_COMMAND);
ev.commandInt=(player) ? player_item->FindString(player->GetName()) : 0;
player_item->Command(ev);
// Fake an infoset selection
if (infoset)
{
	wxCommandEvent ev(wxEVENT_TYPE_CHOICE_COMMAND);
	ev.commandInt=iset_item->FindString(infoset->GetName());
	iset_item->Command(ev);
}
// Start the dialog
Go1();
}

NodeAddDialog::~NodeAddDialog(void)
{delete [] player_name;delete [] iset_name;}

//*********************************** DATA ACCESS *****************************
NodeAddMode NodeAddDialog::GetAddMode(void) const
{return (iset_item->GetSelection()==0) ? NodeAddNew : NodeAddIset;}

EFPlayer *NodeAddDialog::GetPlayer(void)
{
EFPlayer *player;
if (strcmp(player_name,"New Player")==0)
{
	player=ef.NewPlayer();
	player->SetName(ToString(ef.NumPlayers()));
}
else player=EfgGetPlayer(ef,player_name);
if (strcmp(player_name,"Chance")==0) player=ef.GetChance();
return player;
}

Infoset *NodeAddDialog::GetInfoset(void)
{
if (GetAddMode()==NodeAddIset)  {
  for (int iset = 1; iset <= EfgGetPlayer(ef, player_name)->NumInfosets(); iset++)
    if (EfgGetPlayer(ef, player_name)->Infosets()[iset]->GetName() == iset_name)
      return EfgGetPlayer(ef,player_name)->Infosets()[iset];
}
return 0;
}

int NodeAddDialog::GetBranches(void)
{return branches;}

Bool NodeAddDialog::SetNames(void) const
{return set_names;}
//***************************** EVENT HANDLERS-HIGH LEVEL *********************
void NodeAddDialog::OnPlayer(const char *name)
{
iset_item->Clear();
iset_item->Append("New");
EFPlayer *player=EfgGetPlayer(ef,name);
if (player)
	for (int i=1;i<=player->NumInfosets();i++) iset_item->Append(player->Infosets()[i]->GetName());
iset_item->SetSelection(iset_item->FindString("New"));
iset_item->SetSize(-1,-1,-1,-1); // force it to resize
}

void NodeAddDialog::OnIset(const char *name)
{
EFPlayer *player=EfgGetPlayer(ef,player_item->GetStringSelection());
if (!player) return;
Infoset *infoset = 0;
for (int iset = 1; iset <= player->NumInfosets(); iset++)
  if (player->Infosets()[iset]->GetName() == name)  {
    infoset = player->Infosets()[iset];
    break;
  }

if (!infoset) return;
branch_item->SetValue(ToString(infoset->NumActions()));
}

void NodeAddDialog::OnBranch(void)
{
if (iset_item->GetSelection()!=0) iset_item->SetSelection(0);
}

//***************************** EVENT HANDLERS-LOW LEVEL **********************
void NodeAddDialog::player_func(wxChoice &ob,wxCommandEvent &)
{
NodeAddDialog *parent=(NodeAddDialog *)ob.GetClientData();
parent->OnPlayer(ob.GetStringSelection());
}

void NodeAddDialog::iset_func(wxChoice &ob,wxCommandEvent &)
{
NodeAddDialog *parent=(NodeAddDialog *)ob.GetClientData();
parent->OnIset(ob.GetStringSelection());
}

void NodeAddDialog::branch_func(wxText &ob,wxCommandEvent &)
{
NodeAddDialog *parent=(NodeAddDialog *)ob.GetClientData();
parent->OnBranch();
}




