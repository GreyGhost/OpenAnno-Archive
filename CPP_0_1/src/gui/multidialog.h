#ifndef MULTIDIALOG_H_
#define MULTIDIALOG_H_

#include <string>
#include "gui/imagebutton.h"
#include "gui/dialog.h"
#include "gui/events/eventcommand.h"

class MultiDialog : public Dialog
{
public:
	MultiDialog(int x, int y, int width, int height, const std::string& background, int searchCommId, int loginCommId, int registerCommId);
	~MultiDialog();
private:
	void onCommand(EventCommand* event);	
	ImageButton* m_logindialog;
	ImageButton* m_registerdialog;
	ImageButton* m_newsession;
	ImageButton* m_loadsession;
	ImageButton* m_searchgames;
	ImageButton* m_Ok;
	ImageButton* m_Cancel;
	int m_loginCommandId;
	int m_registerCommandId;
	int m_searchCommandId;	
};
#endif /*MULTIDIALOG_H_*/
