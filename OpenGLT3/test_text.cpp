#include "OpenGLGraph.h"
#define SYSCOLOR 0xff0000
#define TEXTLEN  200
#define CURSOR "|"   
#define CURSOR_BLINK  1 

typedef int BOOL;
const int timerseconds = 500;

void KeyboardEventProcess(int key, int event);
void CharEventProcess(char c);
void TimerEventProcess(int timerID);

double tx, ty;
int curPos;
int isDisplayed;

static char textbuf[TEXTLEN + 1];

void InsertCharToString(char *str, int pos, char c);
void DeleteCharFromString(char *str, int pos);
void DrawCursor(char *str, int curPos, double startx, double starty);

char *font;
void Main()
{
	InitGraphics();

	registerKeyboardEvent(KeyboardEventProcess);
	registerCharEvent(CharEventProcess);
	registerTimerEvent(TimerEventProcess);

	tx = 1.0;
	ty = 2.0;
	MovePen(tx, ty);

	SetPenColor(SYSCOLOR);
	SetPenSize(8);

	curPos = 0;
	isDisplayed = TRUE;
	startTimer(CURSOR_BLINK, timerseconds);
	//DrawTextString("HI!!");
}

void KeyboardEventProcess(int key, int event)
{
	switch (event) {
	case KEY_DOWN: {
		switch (key) {
		case VK_LEFT:
			SetEraseMode(TRUE);
			MovePen(tx, ty);
			DrawTextString(textbuf);
			DrawCursor(textbuf, curPos, tx, ty);
			if (curPos>0) curPos--;
			SetEraseMode(FALSE);
			MovePen(tx, ty);
			DrawTextString(textbuf);
			DrawCursor(textbuf, curPos, tx, ty);
			break;
		case VK_RIGHT:
			SetEraseMode(TRUE);
			MovePen(tx, ty);
			DrawTextString(textbuf);
			DrawCursor(textbuf, curPos, tx, ty);
			if (curPos<strlen(textbuf)) curPos++;
			SetEraseMode(FALSE);
			MovePen(tx, ty);
			DrawTextString(textbuf);
			DrawCursor(textbuf, curPos, tx, ty);
			break;
		case VK_DELETE:
			SetEraseMode(TRUE);
			MovePen(tx, ty);
			DrawTextString(textbuf);
			DrawCursor(textbuf, curPos, tx, ty);
			DeleteCharFromString(textbuf, curPos);
			SetEraseMode(FALSE);
			MovePen(tx, ty);
			DrawTextString(textbuf);
			DrawCursor(textbuf, curPos, tx, ty);
			break;
		}
		break;
	}
	}
}

void CharEventProcess(char c)
{
	int len;
	switch (c) {
	case '\r': {
		//InitConsole();
		printf("%s", textbuf);
		//FreeConsole();
		break;
	}
	case '\b':
		if ((len = strlen(textbuf)) == 0) break;
		SetEraseMode(TRUE);
		MovePen(tx, ty);
		DrawTextString(textbuf);
		DrawCursor(textbuf, curPos, tx, ty);
		DeleteCharFromString(textbuf, curPos - 1);
		SetEraseMode(FALSE);
		MovePen(tx, ty);
		DrawTextString(textbuf);
		if (curPos > 0) curPos--;
		DrawCursor(textbuf, curPos, tx, ty);
		break;

	default:
		if ((len = strlen(textbuf)) >= TEXTLEN) break;
		SetEraseMode(TRUE);
		MovePen(tx, ty);
		DrawTextString(textbuf);
		DrawCursor(textbuf, curPos, tx, ty);
		InsertCharToString(textbuf, curPos, c);
		SetEraseMode(FALSE);
		MovePen(tx, ty);
		DrawTextString(textbuf);
		curPos++;
		DrawCursor(textbuf, curPos, tx, ty);
		break;
	}

}

void TimerEventProcess(int timerID)
{
	switch (timerID) {
	case CURSOR_BLINK:
		SetEraseMode(!isDisplayed);
		DrawCursor(textbuf, curPos, tx, ty);
		SetEraseMode(FALSE);
		MovePen(tx, ty);
		DrawTextString(textbuf);
		isDisplayed = !isDisplayed;
		break;
	default:
		break;
	}
}


void InsertCharToString(char *str, int pos, char c)
{
	int len;

	if (pos < 0 || pos >= TEXTLEN) return;
	len = strlen(str);
	*(str + len + 1) = '\0';
	while (len > pos) {
		*(str + len) = *(str + len - 1);
		len--;
	}
	*(str + len) = c;
	return;
}

void DeleteCharFromString(char *str, int pos)
{
	int len;

	len = strlen(str);
	if (pos < 0 || pos >= len) return;

	while (*(str + pos) != '\0') {
		*(str + pos) = *(str + pos + 1);
		pos++;
	}
	return;
}

void DrawCursor(char *str, int curPos, double startx, double starty)
{
	if (curPos < 0 || curPos > strlen(str)) return;
	//MovePen(startx + TextStringWidth(SubString(str, 0, curPos - 1)), starty);
	MovePen(startx, starty);
	DrawTextString(CURSOR);
	return;
}
