#pragma once
#include "Control.h"
#include "Tooltip.h"

class TextBox: public Control
{
public:
	~TextBox();

	//Text limit
	size_t getTextLengthLimit();
	void setTextLengthLimit(size_t limit);

	//Methods for multiline textboxes
	int getCurrentLineIndex();
	int getLineCount();

	//Get / set caret postion
	int getCaretPos();
	void setCarePos(int h, int v);

	//Text selection
	void selectText(size_t startPos, size_t endPos);

	//Tooltip methods
	void showBalloonTip(Tooltip* tip);
};

