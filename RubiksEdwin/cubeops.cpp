#include "stdafx.h"
#include "cubeops.h"

void performop(int cubemap[9][12], int cubex, int cubey, int cubez, int action, int axis, int direction)
{
	switch (axis)
	{
	case 0:
		movevertstrip(cubemap, 4 + cubex, direction);

		if (4 + cubex == 3)
			facerotate(cubemap, 0, 3, direction);
		else if (4 + cubex == 5)
			facerotate(cubemap, 6, 3, -direction);
		break;
	case 1:
		moveradialstrip(cubemap, cubey, direction);

		if (cubey>0)
			facerotate(cubemap, 3, 3, -direction);
		else if (cubey<0)
			facerotate(cubemap, 9, 3, direction);
		break;
	case 2:
		movehoristrip(cubemap, 4 - cubez, direction);

		if (4 - cubez == 3)
			facerotate(cubemap, 3, 0, -direction);
		else if (4 - cubez == 5)
			facerotate(cubemap, 3, 6, direction);
		break;

	}

}

void facerotate(int cubemap[9][12], int facex, int facey, int direction)
{
	int i, j;
	int temp[3][3];

	for (i = 0; i<3; i++)
		for (j = 0; j<3; j++)
			temp[j][i] = cubemap[facey + j][facex + i];

	if (direction>0)
	{
		cubemap[facey][facex] = temp[2][0];
		cubemap[facey + 1][facex] = temp[2][1];
		cubemap[facey + 2][facex] = temp[2][2];

		cubemap[facey][facex + 1] = temp[1][0];
		cubemap[facey + 2][facex + 1] = temp[1][2];

		cubemap[facey][facex + 2] = temp[0][0];
		cubemap[facey + 1][facex + 2] = temp[0][1];
		cubemap[facey + 2][facex + 2] = temp[0][2];
	}

	if (direction<0)
	{
		cubemap[facey][facex] = temp[0][2];
		cubemap[facey + 1][facex] = temp[0][1];
		cubemap[facey + 2][facex] = temp[0][0];

		cubemap[facey][facex + 1] = temp[1][2];
		cubemap[facey + 2][facex + 1] = temp[1][0];

		cubemap[facey][facex + 2] = temp[2][2];
		cubemap[facey + 1][facex + 2] = temp[2][1];
		cubemap[facey + 2][facex + 2] = temp[2][0];
	}
}

void swapstrip(int cubemap[9][12], int strip1x, int strip1y, int strip1xx, int strip1yy, int strip2x, int strip2y, int strip2xx, int strip2yy)
{
	int i;
	int x[3], y[3], temp[3];

	if (strip1x - strip1xx == 0)
	{
		for (i = 0; i<3; i++)
		{
			x[i] = strip1y + i*(strip1yy - strip1y) / 2;
			y[i] = strip1x;
			temp[i] = cubemap[strip1y + i*(strip1yy - strip1y) / 2][strip1x];
		}
	}
	else if (strip1y - strip1yy == 0)
	{
		for (i = 0; i<3; i++)
		{
			x[i] = strip1y;
			y[i] = strip1x + i*(strip1xx - strip1x) / 2;
			temp[i] = cubemap[strip1y][strip1x + i*(strip1xx - strip1x) / 2];
		}
	}


	// Copy Values into correct places
	if (strip2x - strip2xx == 0)
	{
		for (i = 0; i<3; i++)
		{
			cubemap[x[i]][y[i]] = cubemap[strip2y + i*(strip2yy - strip2y) / 2][strip2x];
			cubemap[strip2y + i*(strip2yy - strip2y) / 2][strip2x] = temp[i];
		}
	}
	else if (strip2y - strip2yy == 0)
	{
		for (i = 0; i<3; i++)
		{
			cubemap[x[i]][y[i]] = cubemap[strip2y][strip2x + i*(strip2xx - strip2x) / 2];
			cubemap[strip2y][strip2x + i*(strip2xx - strip2x) / 2] = temp[i];
		}
	}
}

void moveradialstrip(int cubemap[9][12], int stripz, int direction)
{
	if (stripz>1 || stripz<-1)
		return;

	if (direction>0)
	{
		swapstrip(cubemap, 1 + stripz, 3, 1 + stripz, 5, 5, 1 + stripz, 3, 1 + stripz);
		swapstrip(cubemap, 5, 1 + stripz, 3, 1 + stripz, 7 - stripz, 5, 7 - stripz, 3);
		swapstrip(cubemap, 7 - stripz, 5, 7 - stripz, 3, 3, 7 - stripz, 5, 7 - stripz);

	}
	else if (direction<0)
	{
		swapstrip(cubemap, 3, 1 + stripz, 5, 1 + stripz, 1 + stripz, 5, 1 + stripz, 3);
		swapstrip(cubemap, 1 + stripz, 5, 1 + stripz, 3, 5, 7 - stripz, 3, 7 - stripz);
		swapstrip(cubemap, 5, 7 - stripz, 3, 7 - stripz, 7 - stripz, 3, 7 - stripz, 5);
	}

}
void movevertstrip(int cubemap[9][12], int stripx, int direction)
{
	if (stripx>5 || stripx<3)
		return;

	if (direction<0)
	{
		swapstrip(cubemap, stripx, 0, stripx, 2, stripx, 3, stripx, 5);
		swapstrip(cubemap, stripx, 3, stripx, 5, stripx, 6, stripx, 8);
		swapstrip(cubemap, stripx, 6, stripx, 8, 11 - (stripx - 3), 5, 11 - (stripx - 3), 3);
		//		swapstrip(cubemap, 11-(stripx-3),5,11-(stripx-3),3, stripx       ,0,stripx       ,2);
	}
	else if (direction>0)
	{
		swapstrip(cubemap, stripx, 6, stripx, 8, stripx, 3, stripx, 5);
		swapstrip(cubemap, stripx, 3, stripx, 5, stripx, 0, stripx, 2);
		swapstrip(cubemap, stripx, 0, stripx, 2, 11 - (stripx - 3), 5, 11 - (stripx - 3), 3);
		//		swapstrip(cubemap, stripx       ,0,stripx       ,2, 11-(stripx-3),5,11-(stripx-3),3);
	}
}

void movehoristrip(int cubemap[9][12], int stripy, int direction)
{
	int i;
	int temp[3];

	if (direction<0)
	{
		for (i = 0; i<3; i++)
			temp[i] = cubemap[stripy][i];

		for (i = 0; i<9; i++)
			cubemap[stripy][i] = cubemap[stripy][i + 3];

		for (i = 0; i<3; i++)
			cubemap[stripy][9 + i] = temp[i];
	}
	else if (direction>0)
	{
		for (i = 0; i<3; i++)
			temp[i] = cubemap[stripy][9 + i];

		for (i = 0; i<9; i++)
			cubemap[stripy][11 - i] = cubemap[stripy][8 - i];

		for (i = 0; i<3; i++)
			cubemap[stripy][i] = temp[i];
	}
}

