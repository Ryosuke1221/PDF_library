#include "hpdf.h"
#include<stdio.h>
#include<csetjmp>
#include <string.h>
#include "hpdf_config.h"
#include<iostream>

using namespace std;

jmp_buf env;

void
print_grid(HPDF_Doc     pdf,
	HPDF_Page    page)
{
	HPDF_REAL height = HPDF_Page_GetHeight(page);
	HPDF_REAL width = HPDF_Page_GetWidth(page);
	HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", NULL);
	HPDF_UINT x, y;

	HPDF_Page_SetFontAndSize(page, font, 5);
	HPDF_Page_SetGrayFill(page, 0.5);
	HPDF_Page_SetGrayStroke(page, 0.8);

	/* Draw horizontal lines */
	y = 0;
	while (y < height) {
		if (y % 10 == 0)
			HPDF_Page_SetLineWidth(page, 0.5);
		else {
			if (HPDF_Page_GetLineWidth(page) != 0.25)
				HPDF_Page_SetLineWidth(page, 0.25);
		}

		HPDF_Page_MoveTo(page, 0, y);
		HPDF_Page_LineTo(page, width, y);
		HPDF_Page_Stroke(page);

		if (y % 10 == 0 && y > 0) {
			HPDF_Page_SetGrayStroke(page, 0.5);

			HPDF_Page_MoveTo(page, 0, y);
			HPDF_Page_LineTo(page, 5, y);
			HPDF_Page_Stroke(page);

			HPDF_Page_SetGrayStroke(page, 0.8);
		}

		y += 5;
	}


	/* Draw virtical lines */
	x = 0;
	while (x < width) {
		if (x % 10 == 0)
			HPDF_Page_SetLineWidth(page, 0.5);
		else {
			if (HPDF_Page_GetLineWidth(page) != 0.25)
				HPDF_Page_SetLineWidth(page, 0.25);
		}

		HPDF_Page_MoveTo(page, x, 0);
		HPDF_Page_LineTo(page, x, height);
		HPDF_Page_Stroke(page);

		if (x % 50 == 0 && x > 0) {
			HPDF_Page_SetGrayStroke(page, 0.5);

			HPDF_Page_MoveTo(page, x, 0);
			HPDF_Page_LineTo(page, x, 5);
			HPDF_Page_Stroke(page);

			HPDF_Page_MoveTo(page, x, height);
			HPDF_Page_LineTo(page, x, height - 5);
			HPDF_Page_Stroke(page);

			HPDF_Page_SetGrayStroke(page, 0.8);
		}

		x += 5;
	}

	/* Draw horizontal text */
	y = 0;
	while (y < height) {
		if (y % 10 == 0 && y > 0) {
			char buf[12];

			HPDF_Page_BeginText(page);
			HPDF_Page_MoveTextPos(page, 5, y - 2);
#ifdef __WIN32__
			_snprintf(buf, 12, "%u", y);
#else
			snprintf(buf, 12, "%u", y);
#endif
			HPDF_Page_ShowText(page, buf);
			HPDF_Page_EndText(page);
		}

		y += 5;
	}


	/* Draw virtical text */
	x = 0;
	while (x < width) {
		if (x % 50 == 0 && x > 0) {
			char buf[12];

			HPDF_Page_BeginText(page);
			HPDF_Page_MoveTextPos(page, x, 5);
#ifdef __WIN32__
			_snprintf(buf, 12, "%u", x);
#else
			snprintf(buf, 12, "%u", x);
#endif
			HPDF_Page_ShowText(page, buf);
			HPDF_Page_EndText(page);

			HPDF_Page_BeginText(page);
			HPDF_Page_MoveTextPos(page, x, height - 10);
			HPDF_Page_ShowText(page, buf);
			HPDF_Page_EndText(page);
		}

		x += 5;
	}

	HPDF_Page_SetGrayFill(page, 0);
	HPDF_Page_SetGrayStroke(page, 0);
}

#ifdef HPDF_DLL
void  __stdcall
#else
void
#endif
error_handler(HPDF_STATUS   error_no,
	HPDF_STATUS   detail_no,
	void         *user_data)
{
	printf("ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT)error_no,
		(HPDF_UINT)detail_no);
	longjmp(env, 1);
}

int main(int argc, char **argv)
{
	HPDF_Doc  pdf;
	HPDF_Page page;
	char fname[256];
	HPDF_Point pos;

	strcpy(fname, argv[0]);
	strcat(fname, ".pdf");

	pdf = HPDF_New(error_handler, NULL);
	if (!pdf) {
		printf("error: cannot create PdfDoc object\n");
		cout << "1" << endl;
		return 1;
	}

	if (setjmp(env)) {
		HPDF_Free(pdf);
		cout << "2" << endl;

		return 1;
	}

	cout << "3" << endl;


	/* add a new page object. */
	page = HPDF_AddPage(pdf);

	HPDF_Page_SetHeight(page, 220);
	HPDF_Page_SetWidth(page, 200);

	/* draw grid to the page */
	print_grid(pdf, page);

	/* draw pie chart
	 *
	 *   A: 45% Red
	 *   B: 25% Blue
	 *   C: 15% green
	 *   D: other yellow
	 */

	 /* A */
	HPDF_Page_SetRGBFill(page, 1.0, 0, 0);
	HPDF_Page_MoveTo(page, 100, 100);
	HPDF_Page_LineTo(page, 100, 180);
	HPDF_Page_Arc(page, 100, 100, 80, 0, 360 * 0.45);
	pos = HPDF_Page_GetCurrentPos(page);
	HPDF_Page_LineTo(page, 100, 100);
	HPDF_Page_Fill(page);

	/* B */
	HPDF_Page_SetRGBFill(page, 0, 0, 1.0);
	HPDF_Page_MoveTo(page, 100, 100);
	HPDF_Page_LineTo(page, pos.x, pos.y);
	HPDF_Page_Arc(page, 100, 100, 80, 360 * 0.45, 360 * 0.7);
	pos = HPDF_Page_GetCurrentPos(page);
	HPDF_Page_LineTo(page, 100, 100);
	HPDF_Page_Fill(page);

	/* C */
	HPDF_Page_SetRGBFill(page, 0, 1.0, 0);
	HPDF_Page_MoveTo(page, 100, 100);
	HPDF_Page_LineTo(page, pos.x, pos.y);
	HPDF_Page_Arc(page, 100, 100, 80, 360 * 0.7, 360 * 0.85);
	pos = HPDF_Page_GetCurrentPos(page);
	HPDF_Page_LineTo(page, 100, 100);
	HPDF_Page_Fill(page);

	/* D */
	HPDF_Page_SetRGBFill(page, 1.0, 1.0, 0);
	HPDF_Page_MoveTo(page, 100, 100);
	HPDF_Page_LineTo(page, pos.x, pos.y);
	HPDF_Page_Arc(page, 100, 100, 80, 360 * 0.85, 360);
	pos = HPDF_Page_GetCurrentPos(page);
	HPDF_Page_LineTo(page, 100, 100);
	HPDF_Page_Fill(page);

	/* draw center circle */
	HPDF_Page_SetGrayStroke(page, 0);
	HPDF_Page_SetGrayFill(page, 1);
	HPDF_Page_Circle(page, 100, 100, 30);
	HPDF_Page_Fill(page);

	/* save the document to a file */
	HPDF_SaveToFile(pdf, fname);

	/* clean up */
	HPDF_Free(pdf);

	return 0;
}