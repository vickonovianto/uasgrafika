/* Functions that provide the backend to the rubiks cube program



*/
#include "stdafx.h"
#include "driver3.h"


//presumes that the following is defined, would be commented out when real program comes around
int colorMatrix[12][9];
int tmpMatrix[3][3];
int current_face;
//
//void getInput(int /*face*/, int /*sticker*/, int /*direction*/);
//int getColor(int /*face*/, int /*sticker*/);
//void rotate2Front(int);
//void rotateback(void);
//void move(int,int);
//void rotate(int,bool);
//void rotateSliver(bool,int,int,int);
//void PeelOffStickers();

/*int main(void)
{
//do stuff, just here for effect, and compilation
PeelOffStickers();
return 0;
}
*/

void getInput(int face, int sticker, int direction, int *cubestate)
{
	memcpy(colorMatrix, cubestate, sizeof(int) * 9 * 12);

	//The matrix is defined with the primary face being the center face
	PeelOffStickers();

	rotate2Front(face);

	switch (direction)
	{
	case 1: //up
		switch (sticker)
		{
		case 1: case 4: case 7: //left side
			rotate(1, false);
			rotateSliver(false, true, 0, 0);
			break;
		case 2: case 5: case 8: //middle sliver
			rotateSliver(false, true, 1, 0);
			break;
		case 3: case 6: case 9: //right sliver
			rotate(5, true);
			rotateSliver(false, true, 2, 0);
			break;
		}
		break;
	case 3: //down
		switch (sticker)
		{
		case 1: case 4: case 7: //left side
			rotate(1, true);
			rotateSliver(false, false, 0, 0);
			break;
		case 2: case 5: case 8: //middle sliver
			rotateSliver(false, false, 1, 0);
			break;
		case 3: case 6: case 9: //right sliver
			rotate(5, false);
			rotateSliver(false, false, 2, 0);
			break;
		}
		break;
	case 2: //right
		switch (sticker)
		{
		case 1: case 2: case 3:
			rotate(2, false);
			rotateSliver(true, true, 0, 0);
			break;
		case 4: case 5: case 6:
			rotateSliver(true, true, 0, 1);
			break;
		case 7: case 8: case 9:
			rotate(4, true);
			rotateSliver(true, true, 0, 2);
			break;
		}
		break;
	case 4: //left
		switch (sticker)
		{
		case 1: case 2: case 3:
			rotate(2, true);
			rotateSliver(true, false, 0, 0);
			break;
		case 4: case 5: case 6:
			rotateSliver(true, false, 0, 1);
			break;
		case 7: case 8: case 9:
			rotate(4, false);
			rotateSliver(true, false, 0, 2);
			break;
		}
		break;
	default: break;
	}

	rotateback();

	memcpy(cubestate, colorMatrix, sizeof(int) * 9 * 12);

	return;
}

void rotate2Front(int /*current face*/ face)
{
	current_face = face;
	switch (face)
	{
	case 3: //already in the front, so do nothing
		break;
	case 1: //left of front face, mustrotate to the right, moving 3->5->6->1, then rotate 2 ccwise, and 4 cwise
		move(3, 0); //moving face 3 to temp
		move(1, 3); //moving face 1 to spot 3
		move(6, 1);
		move(5, 6);
		move(0, 5);
		rotate(2, false); //rotate face 2 counterclockwise
		rotate(4, true); //rotate face 4 clockwise
		break;
	case 2: //2->3->4->6->2, rotate 1 cwise, rotate 5 ccw
		move(3, 0);
		move(2, 3);
		move(6, 2);
		move(4, 6);
		move(0, 4);
		rotate(1, true);
		rotate(5, false);
		break;
	case 4: //3->2->6->4->3, rotate 1 ccw, rotate 5 cwise
		move(3, 0);
		move(4, 3);
		move(6, 4);
		move(2, 6);
		move(0, 2);
		rotate(1, false);
		rotate(5, true);
		break;
	case 5: //3->1->6->5->3, rotate 2 cwise, rotate 4 ccw
		move(3, 0);
		move(5, 3);
		move(6, 5);
		move(1, 6);
		move(0, 1);
		rotate(4, false);
		rotate(2, true);
		break;
	case 6: //
		rotate2Front(4);
		rotate2Front(4);
		current_face = 6;
		break;
	}//end switch
	return;
}

void rotateback(void)
{
	rotate2Front(current_face);
	return;
}

void move(int first, int second)
{
	bool notTemp;
	int startx;
	int starty;
	int tmp2[3][3];

	switch (first)
	{
	case 0:
		tmp2[0][0] = tmpMatrix[0][0];	tmp2[0][1] = tmpMatrix[0][1];	tmp2[0][2] = tmpMatrix[0][2];
		tmp2[1][0] = tmpMatrix[1][0];	tmp2[1][1] = tmpMatrix[1][1];	tmp2[1][2] = tmpMatrix[1][2];
		tmp2[2][0] = tmpMatrix[2][0];	tmp2[2][1] = tmpMatrix[2][1];	tmp2[2][2] = tmpMatrix[2][2];
		notTemp = false;
		break;
	case 1:
		startx = 0;
		starty = 3;
		notTemp = true;
		break;
	case 2:
		startx = 3;
		starty = 0;
		notTemp = true;
		break;
	case 3:
		startx = 3;
		starty = 3;
		notTemp = true;
		break;
	case 4:
		startx = 3;
		starty = 6;
		notTemp = true;
		break;
	case 5:
		startx = 6;
		starty = 3;
		notTemp = true;
		break;
	case 6:
		startx = 9;
		starty = 3;
		notTemp = true;
		break;
	default:
		break;
	}

	if (notTemp)
	{
		tmp2[0][0] = tmpMatrix[startx][starty];	tmp2[1][0] = tmpMatrix[startx + 1][starty];	tmp2[2][0] = tmpMatrix[startx + 2][starty];
		tmp2[0][1] = tmpMatrix[startx][starty + 1];	tmp2[1][1] = tmpMatrix[startx + 1][starty + 1];	tmp2[2][1] = tmpMatrix[startx + 2][starty + 1];
		tmp2[0][2] = tmpMatrix[startx][starty + 2];	tmp2[1][2] = tmpMatrix[startx + 1][starty + 2];	tmp2[2][2] = tmpMatrix[startx + 2][starty + 2];
	}

	switch (second)
	{
	case 0:
		tmpMatrix[0][0] = tmp2[0][0];	tmpMatrix[0][1] = tmp2[0][1];	tmpMatrix[0][2] = tmp2[0][2];
		tmpMatrix[1][0] = tmp2[1][0];	tmpMatrix[1][1] = tmp2[1][1];	tmpMatrix[1][2] = tmp2[1][2];
		tmpMatrix[2][0] = tmp2[2][0];	tmpMatrix[2][1] = tmp2[2][1];	tmpMatrix[2][2] = tmp2[2][2];
		notTemp = false;
		break;
	case 1:
		startx = 0;
		starty = 3;
		notTemp = true;
		break;
	case 2:
		startx = 3;
		starty = 0;
		notTemp = true;
		break;
	case 3:
		startx = 3;
		starty = 3;
		notTemp = true;
		break;
	case 4:
		startx = 3;
		starty = 6;
		notTemp = true;
		break;
	case 5:
		startx = 6;
		starty = 3;
		notTemp = true;
		break;
	case 6:
		startx = 9;
		starty = 3;
		notTemp = true;
		break;
	default:
		break;
	}

	if (notTemp)
	{
		colorMatrix[startx][starty] = tmp2[0][0];	colorMatrix[startx + 1][starty] = tmp2[1][0];	colorMatrix[startx + 2][starty] = tmp2[2][0];
		colorMatrix[startx][starty + 1] = tmp2[0][1];	colorMatrix[startx + 1][starty + 1] = tmp2[1][1];	colorMatrix[startx + 2][starty + 1] = tmp2[2][1];
		colorMatrix[startx][starty + 2] = tmp2[0][2];	colorMatrix[startx + 1][starty + 2] = tmp2[1][2];	colorMatrix[startx + 2][starty + 2] = tmp2[2][2];
	}

	return;
}

void rotate(int face, bool clockwise) //rotates the selcted face clockwise or counterclockwise, not mattering about the faces near
{
	int temp, startx, starty;
	bool notTemp;

	switch (face)
	{
	case 1:
		startx = 0;
		starty = 3;
		notTemp = true;
		break;
	case 2:
		startx = 3;
		starty = 0;
		notTemp = true;
		break;
	case 3:
		startx = 3;
		starty = 3;
		notTemp = true;
		break;
	case 4:
		startx = 3;
		starty = 6;
		notTemp = true;
		break;
	case 5:
		startx = 6;
		starty = 3;
		notTemp = true;
		break;
	case 6:
		startx = 9;
		starty = 3;
		notTemp = true;
		break;
	default:
		break;
	}

	if (clockwise)
	{
		temp = colorMatrix[startx][starty];
		colorMatrix[startx][starty] = colorMatrix[startx][starty + 2];
		colorMatrix[startx][starty + 2] = colorMatrix[startx + 2][starty + 2];
		colorMatrix[startx + 2][starty + 2] = colorMatrix[startx + 2][starty];
		colorMatrix[startx + 2][starty] = temp;

		temp = colorMatrix[startx + 1][starty];
		colorMatrix[startx + 1][starty] = colorMatrix[startx][starty + 1];
		colorMatrix[startx][starty + 1] = colorMatrix[startx + 1][starty + 1];
		colorMatrix[startx + 1][starty + 1] = colorMatrix[startx + 2][starty + 1];
		colorMatrix[startx + 2][starty + 1] = temp;
	}
	else
	{
		rotate(face, true);
		rotate(face, true);
		rotate(face, true);
	}
	return;
}

void rotateSliver(bool xdir, bool right /*or up*/, int offset_x, int offset_y)
{
	int temp[1][3];
	int temp2[3][1];

	if (xdir)
	{
		if (right)
		{
			temp[0][0] = colorMatrix[3][4 + offset_y - 1];
			temp[1][0] = colorMatrix[4][4 + offset_y - 1];
			temp[2][0] = colorMatrix[51][4 + offset_y - 1];

			colorMatrix[3][4 + offset_y - 1] = colorMatrix[0][4 + offset_y - 1];
			colorMatrix[4][4 + offset_y - 1] = colorMatrix[1][4 + offset_y - 1];
			colorMatrix[5][4 + offset_y - 1] = colorMatrix[2][4 + offset_y - 1];

			colorMatrix[0][4 + offset_y - 1] = colorMatrix[9][4 + offset_y - 1];
			colorMatrix[1][4 + offset_y - 1] = colorMatrix[10][4 + offset_y - 1];
			colorMatrix[2][4 + offset_y - 1] = colorMatrix[11][4 + offset_y - 1];

			colorMatrix[9][4 + offset_y - 1] = colorMatrix[6][4 + offset_y - 1];
			colorMatrix[10][4 + offset_y - 1] = colorMatrix[7][4 + offset_y - 1];
			colorMatrix[11][4 + offset_y - 1] = colorMatrix[8][4 + offset_y - 1];

			colorMatrix[6][4 + offset_y - 1] = temp[0][0];
			colorMatrix[7][4 + offset_y - 1] = temp[1][0];
			colorMatrix[8][4 + offset_y - 1] = temp[2][0];
		}
		else
		{
			temp[0][0] = colorMatrix[3][4 + offset_y - 1];
			temp[1][0] = colorMatrix[4][4 + offset_y - 1];
			temp[2][0] = colorMatrix[5][4 + offset_y - 1];

			colorMatrix[3][4 + offset_y - 1] = colorMatrix[6][4 + offset_y - 1];
			colorMatrix[4][4 + offset_y - 1] = colorMatrix[7][4 + offset_y - 1];
			colorMatrix[5][4 + offset_y - 1] = colorMatrix[8][4 + offset_y - 1];

			colorMatrix[6][4 + offset_y - 1] = colorMatrix[9][4 + offset_y - 1];
			colorMatrix[7][4 + offset_y - 1] = colorMatrix[10][4 + offset_y - 1];
			colorMatrix[8][4 + offset_y - 1] = colorMatrix[11][4 + offset_y - 1];

			colorMatrix[9][4 + offset_y - 1] = colorMatrix[0][4 + offset_y - 1];
			colorMatrix[10][4 + offset_y - 1] = colorMatrix[1][4 + offset_y - 1];
			colorMatrix[11][4 + offset_y - 1] = colorMatrix[2][4 + offset_y - 1];

			colorMatrix[0][4 + offset_y - 1] = temp[0][0];
			colorMatrix[1][4 + offset_y - 1] = temp[1][0];
			colorMatrix[2][4 + offset_y - 1] = temp[2][0];
		}
	}
	else
	{
		if (right) //or up
		{
			temp[0][0] = colorMatrix[4 + offset_x - 1][3];
			temp[0][1] = colorMatrix[4 + offset_x - 1][4];
			temp[0][2] = colorMatrix[4 + offset_x - 1][5];

			colorMatrix[4 + offset_x - 1][3] = colorMatrix[4 + offset_x - 1][6];
			colorMatrix[4 + offset_x - 1][4] = colorMatrix[4 + offset_x - 1][7];
			colorMatrix[4 + offset_x - 1][5] = colorMatrix[4 + offset_x - 1][8];

			colorMatrix[4 + offset_x - 1][6] = colorMatrix[10 + offset_x - 1][3];
			colorMatrix[4 + offset_x - 1][7] = colorMatrix[10 + offset_x - 1][4];
			colorMatrix[4 + offset_x - 1][8] = colorMatrix[10 + offset_x - 1][5];

			colorMatrix[10 + offset_x - 1][3] = colorMatrix[4 + offset_x - 1][0];
			colorMatrix[10 + offset_x - 1][4] = colorMatrix[4 + offset_x - 1][1];
			colorMatrix[10 + offset_x - 1][5] = colorMatrix[4 + offset_x - 1][2];

			colorMatrix[4 + offset_x - 1][0] = temp[0][0];
			colorMatrix[4 + offset_x - 1][1] = temp[0][1];
			colorMatrix[4 + offset_x - 1][2] = temp[0][2];
		}
		else
		{
			temp[0][0] = colorMatrix[4 + offset_x - 1][3];
			temp[0][1] = colorMatrix[4 + offset_x - 1][4];
			temp[0][2] = colorMatrix[4 + offset_x - 1][5];

			colorMatrix[4 + offset_x - 1][3] = colorMatrix[4 + offset_x - 1][0];
			colorMatrix[4 + offset_x - 1][4] = colorMatrix[4 + offset_x - 1][1];
			colorMatrix[4 + offset_x - 1][5] = colorMatrix[4 + offset_x - 1][2];

			colorMatrix[4 + offset_x - 1][0] = colorMatrix[10 + offset_x - 1][3];
			colorMatrix[4 + offset_x - 1][1] = colorMatrix[10 + offset_x - 1][4];
			colorMatrix[4 + offset_x - 1][2] = colorMatrix[10 + offset_x - 1][5];

			colorMatrix[10 + offset_x - 1][3] = colorMatrix[4 + offset_x - 1][6];
			colorMatrix[10 + offset_x - 1][4] = colorMatrix[4 + offset_x - 1][7];
			colorMatrix[10 + offset_x - 1][5] = colorMatrix[4 + offset_x - 1][8];

			colorMatrix[4 + offset_x - 1][6] = temp[0][0];
			colorMatrix[4 + offset_x - 1][7] = temp[0][1];
			colorMatrix[4 + offset_x - 1][8] = temp[0][2];
		}
	}
	return;
}

void PeelOffStickers(void)
{
	bool solved;
	solved = true;
	return;
}

