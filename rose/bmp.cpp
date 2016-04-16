#include "stdafx.h"
#include "bmp.h"

GLuint ATLLoadTexture(const char* fileName)
{

	//char ch[1024] = "wo shi ni baba";
	int num = MultiByteToWideChar(0, 0, fileName, -1, NULL, 0);
	wchar_t *wide = new wchar_t[num];
	MultiByteToWideChar(0, 0, fileName, -1, wide, num);


	BITMAP bm;
	GLuint idTexture = 0;
	CImage img;
	HRESULT hr = img.Load(wide);
	if (!SUCCEEDED(hr))
	{
		cerr << "ÎÄ¼þ¼ÓÔØÊ§°Ü" << endl;
		return NULL;
	}
	HBITMAP hbmp = img;
	if (!GetObject(hbmp, sizeof(bm), &bm))
	{
		return 0;
	}

	glGenTextures(1, &idTexture);
	if (idTexture)
	{
		glBindTexture(GL_TEXTURE_2D, idTexture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glPixelStoref(GL_PACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, bm.bmWidth, bm.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bm.bmBits);

	}

	return idTexture;
}

