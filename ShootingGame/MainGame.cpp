#include "DxLib.h"
struct POINT2DF {
	float x;
	float y;
};
struct MOVE2DF {
	float x;
	float y;
};
struct BULLET2DF {
	struct POINT2DF pos;//座標
	int life = 0;//表示フラグ
	int r = 10;//半径
	struct MOVE2DF move;//速度
};
struct CHARADATA {
	struct POINT2DF pos;//座標
	int life = 0;//体力
	int r;//半径
};

int WINAPI WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR lpC, int nC)
{
	const float dt = 1.0F / 60;
	const float v = 300; // 1 秒あたりの移動速度[ピクセル]
	char keyBuf[256], keyBufOld[256];
	struct CHARADATA player, enemy[5], boss, last, B_end[20];
	struct BULLET2DF P_bullet[30], E_bullet[30];
	struct POINT2DF  E_move[5];

	ChangeWindowMode(TRUE);
	if (DxLib_Init() == -1)
		return -1;
	SetDrawScreen(DX_SCREEN_BACK);

	const int width = 640, height = 480, side = 40;
	unsigned int color[6], R = 15, G = 95, B = 175;//色
	int count = 0, x = 0, y = 0, Esc = 0, l_pattern = 0;
	int	visible = 0, sub_W = 0, HIT[3], B_count = 0, end_flag = 0;
	int enemy_menber, boss_menber, end_rool = 0, back = 641;
	unsigned int scrollCount = 0;
	bool vis = TRUE;

	int destX[2], srcX[2], srcL[2], mapW, mapH;

	int I_enemy, I_boss, I_last, I_player, I_P_bu, I_E_bu;
	I_enemy = LoadGraph(".\\media\\enemy.png");
	I_boss = LoadGraph(".\\media\\boss.png");
	I_last = LoadGraph(".\\media\\last.png");
	I_player = LoadGraph(".\\media\\player.png");
	I_P_bu = LoadGraph(".\\media\\I_P_bu.png");
	I_E_bu = LoadGraph(".\\media\\I_E_bu.png");

	int music[3];
	music[0] = LoadSoundMem(".\\media\\BGM07battle2.wav");
	music[1] = LoadSoundMem(".\\media\\Player_Footstep_09.wav");
	music[2] = LoadSoundMem(".\\media\\爆発1.mp3");

	int	map = LoadGraph(".\\media\\map.png");
	GetGraphSize(map, &mapW, &mapH);

	int bomb[20];
	LoadDivGraph(".\\media\\grenade_Bomb.png", 20, 5, 4, 192, 192, bomb);

	//color[0] = GetColor(R, G, B);//変動
	color[1] = GetColor(255, 255, 255);//白
	color[2] = GetColor(200, 50, 200);//紫
	color[3] = GetColor(64, 128, 128);//深緑
	color[4] = GetColor(255, 0, 0);//赤
	color[5] = GetColor(0, 0, 0);//黒
	unsigned int strat[3][3]{ {0,1,1},{2,1,1},{2,1,1} };

	GetHitKeyStateAll(keyBufOld);

	while (ProcessMessage() != -1) {

		GetHitKeyStateAll(keyBuf);
		if (keyBuf[KEY_INPUT_ESCAPE] == 1)
		{
			Esc = 1;
			break;
		}

		if (keyBuf[KEY_INPUT_SPACE] == 1)
			break;

		if (keyBuf[KEY_INPUT_DOWN] == 1 && keyBufOld[KEY_INPUT_DOWN] == 0) {
			if (y < 2) {
				strat[y][x] = 2;
				y++;

				if (strat[y][0] == 2)
				{
					x = 0;
				}
				else if (strat[y][1] == 2)
				{
					x = 1;
				}
				else if (strat[y][2] == 2)
				{
					x = 2;
				}

				strat[y][x] = 0;
			}

		}

		if (keyBuf[KEY_INPUT_UP] == 1 && keyBufOld[KEY_INPUT_UP] == 0) {
			if (y > 0) {
				strat[y][x] = 2;
				y--;

				if (strat[y][0] == 2)
				{
					x = 0;
				}
				else if (strat[y][1] == 2)
				{
					x = 1;
				}
				else if (strat[y][2] == 2)
				{
					x = 2;
				}

				strat[y][x] = 0;
			}
		}

		if (keyBuf[KEY_INPUT_RIGHT] == 1 && keyBufOld[KEY_INPUT_RIGHT] == 0) {
			if (x < 2) {
				strat[y][x] = 1;

				x++;

				strat[y][x] = 0;
			}

		}

		if (keyBuf[KEY_INPUT_LEFT] == 1 && keyBufOld[KEY_INPUT_LEFT] == 0) {
			if (x > 0) {
				strat[y][x] = 1;

				x--;

				strat[y][x] = 0;
			}

		}

		R++; G++; B++;
		if (R == 255)R = 15;
		if (G == 255)G = 15;
		if (B == 255)B = 15;
		color[0] = GetColor(R, G, B);//変動

		for (int i = 0; i < 256; i++)
			keyBufOld[i] = keyBuf[i];

		//｜｜｜｜描画｜｜｜｜
		DrawBox(0, 0, width, height, color[5], TRUE);

		DrawBox(20, 100, 280, 440, color[1], FALSE);
		for (int i = 0; i < 3; i++)
		{
			DrawBox(320 - i, 100 - i, 400 + i, 200 + i, color[strat[0][0]], FALSE);
			DrawBox(420 - i, 100 - i, 500 + i, 200 + i, color[strat[0][1]], FALSE);
			DrawBox(520 - i, 100 - i, 600 + i, 200 + i, color[strat[0][2]], FALSE);
			DrawBox(320 - i, 220 - i, 400 + i, 320 + i, color[strat[1][0]], FALSE);
			DrawBox(420 - i, 220 - i, 500 + i, 320 + i, color[strat[1][1]], FALSE);
			DrawBox(520 - i, 220 - i, 600 + i, 320 + i, color[strat[1][2]], FALSE);
			DrawBox(320 - i, 340 - i, 400 + i, 440 + i, color[strat[2][0]], FALSE);
			DrawBox(420 - i, 340 - i, 500 + i, 440 + i, color[strat[2][1]], FALSE);
			DrawBox(520 - i, 340 - i, 600 + i, 440 + i, color[strat[2][2]], FALSE);

		}

		DrawFormatString(240, 40, color[3], "シューティングゲーム");
		DrawFormatString(293, 122, color[3], "難\n易\n度\n\n\n\n当\nた\nり\n判\n定\n\n\n\n装\n備");
		DrawFormatString(351, 142, color[1], "並　　　　　上　　　　　極");
		DrawFormatString(340, 262, color[1], "なし　　　　表示　　　 枠のみ");
		DrawFormatString(340, 382, color[1], "なし　　　 バリア　   レーザー");
		DrawFormatString(40, 140, color[3], "　　操作説明\n\n　十字キーで移動\n　SPACEキーで射撃\n　Bキーでオート射撃\n  ESCAPEキーで終了");
		DrawFormatString(85, 380, color[3], "SPACE で START");

		DrawGraph(80, 240, I_player, TRUE);

		ScreenFlip();
	}

	for (int j = 0; j < height / side && Esc == 0; j++)
	{
		if (j % 2 == 0)
		{
			count = 0;
		}
		else
		{
			count = 1;
		}
		for (int i = 0; i < width / side; i++)
		{
			x = i * side;
			y = j * side;
			DrawBox(x, y, x + side, y + side, color[count % 2 + 1], TRUE);
			count++;
			ScreenFlip();
		}
	}

	//ゲーム開始前の処理
	for (int i = 0; i < 3 && Esc == 0; i++) {
		if (strat[0][i] != 1) {
			enemy_menber = 12 * (i + 1);
			boss_menber = 2 + i;
			player.life = 5 + i;
			last.life = 20 + i * 10;
		}

		if (strat[1][i] != 1)
			visible = i;

		if (strat[2][i] != 1)
			sub_W = i;
	}

	if (visible == 1)
		vis = TRUE;
	else if (visible == 2)
		vis = FALSE;

	player.pos = { 200, 80 };
	player.r = 15;

	last.pos = { -200, 240 };
	last.r = 25;

	for (int i = 0; i < 30; i++)
		P_bullet[i].life = 0;

	for (int i = 0; i < 20; i++)
		B_end[i].life = -1;
	PlaySoundMem(music[0], DX_PLAYTYPE_LOOP);

	while (ProcessMessage() != -1 && Esc == 0) {

		GetHitKeyStateAll(keyBuf);
		if (keyBuf[KEY_INPUT_ESCAPE] == 1)
		{
			Esc++;
			break;
		}

		//自機の操作
		if (player.life > 0)
		{
			if (keyBuf[KEY_INPUT_RIGHT] == 1 || keyBuf[KEY_INPUT_D] == 1)
				player.pos.x += v * dt;

			if (keyBuf[KEY_INPUT_LEFT] == 1 || keyBuf[KEY_INPUT_A] == 1)
				player.pos.x -= v * dt;

			if (keyBuf[KEY_INPUT_DOWN] == 1 || keyBuf[KEY_INPUT_S] == 1)
				player.pos.y += v * dt;

			if (keyBuf[KEY_INPUT_UP] == 1 || keyBuf[KEY_INPUT_W] == 1)
				player.pos.y -= v * dt;

			if (keyBuf[KEY_INPUT_SPACE] == 1 && keyBufOld[KEY_INPUT_SPACE] == 0)//弾操作
				for (int i = 0; i < 30; i++)
				{
					if (!P_bullet[i].life)
					{
						P_bullet[i].life = 1;
						P_bullet[i].pos.x = player.pos.x;
						P_bullet[i].pos.y = player.pos.y;
						P_bullet[i].move.x = v * dt * 2;
						P_bullet[i].move.y = 0;
						B_count++;

						PlaySoundMem(music[1], DX_PLAYTYPE_BACK);

						break;
					}
				}
			if (keyBuf[KEY_INPUT_B] == 1)
			{
				if (scrollCount % 5 == 0)
					for (int i = 0; i < 30; i++)
					{
						if (!P_bullet[i].life)
						{
							P_bullet[i].life = 1;
							P_bullet[i].pos.x = player.pos.x;
							P_bullet[i].pos.y = player.pos.y;
							P_bullet[i].move.x = v * dt * 2;
							P_bullet[i].move.y = 0;
							B_count++;

							PlaySoundMem(music[1], DX_PLAYTYPE_BACK);

							break;
						}
					}
			}

		}

		if (player.pos.x < player.r)player.pos.x = player.r;
		if (player.pos.x > -player.r + width - 1)player.pos.x = -player.r + width - 1;
		if (player.pos.y < player.r)player.pos.y = player.r;
		if (player.pos.y > -player.r + height - 1)player.pos.y = -player.r + height - 1;

		//敵の挙動

		for (int i = 0; i < 5; i++) {
			if (!enemy[i].life && enemy_menber > 0) {
				enemy[i].life = 1;
				enemy[i].pos.x = width + player.pos.x;
				enemy[i].pos.y = 180 * i - 120;
				enemy[i].r = 10;
				E_move[i].x = (player.pos.x - enemy[i].pos.x) * dt;
				E_move[i].y = (player.pos.y - enemy[i].pos.y) * dt;
			}

			if (enemy[i].life) {
				enemy[i].pos.x += E_move[i].x;
				enemy[i].pos.y += E_move[i].y;

				if (scrollCount % 200 == 0) {
					E_move[i].x = (player.pos.x - enemy[i].pos.x) * dt / 2;
					E_move[i].y = (player.pos.y - enemy[i].pos.y) * dt / 2;
				}

				HIT[0] = enemy[i].pos.x - player.pos.x;
				HIT[1] = enemy[i].pos.y - player.pos.y;
				HIT[2] = enemy[i].r + player.r;

				if (HIT[0] * HIT[0] + HIT[1] * HIT[1] < HIT[2] * HIT[2])
				{
					if (sub_W != 1)
						player.life--;

					enemy[i].life--;
				}
			}
		}

		if (!boss.life && boss_menber > 0) {
			boss.life = 10;
			boss.pos.x = width * 3 / 2;
			boss.pos.y = 240;
			boss.r = 20;
		}
	
		if (boss.life > 0) {
			if (boss.pos.x >= 560)
				boss.pos.x -= v * dt / 2;
			else
			{
				boss.pos.y -= v * dt / 2;

				if (boss.pos.y < -(boss.r * 2))
					boss.pos.y = height + boss.r * 2;

				if (scrollCount % 80 == 0) {
					for (int j = 0; j < 3; j++)
						for (int i = 0; i < 30; i++)
						{
							if (!E_bullet[i].life)
							{
								E_bullet[i].life = 1;
								E_bullet[i].pos.x = boss.pos.x;
								E_bullet[i].pos.y = boss.pos.y;
								E_bullet[i].r = 13;
								E_bullet[i].move.x = -(v * dt * 3 / 2);
								E_bullet[i].move.y = v * dt * (j - 1);
								break;
							}
						}
				}
			}
		}

		if (enemy_menber <= 0 && boss_menber <= 0 && last.life > 0)
		{
			if (back == 641)
				back--;

			if (last.pos.x <= 380 && l_pattern == 0)
				last.pos.x += v * dt / 2;

			else
			{
				if (l_pattern == 0)
				{
					B_count = 0;
					l_pattern = 1;
					E_move[0].x = 0;
					E_move[0].y = 0;
				}

				for (int i = 0; i < B_count; i++)
					for (int j = 0; j < 30; j++)
					{
						if (E_bullet[j].life == 0)
						{
							E_bullet[j].life = 1;
							E_bullet[j].pos.x = last.pos.x;
							E_bullet[j].pos.y = last.pos.y;
							E_bullet[j].r = 15;
							E_bullet[j].move.x = (player.pos.x - last.pos.x) * dt * 3 / 2;
							E_bullet[j].move.y = (player.pos.y - last.pos.y) * dt * 3 / 2;
							j = +30;
						}

						if (j > 29)
							break;
					}

				B_count = 0;

				if (scrollCount % 200 == 0)
				{
					E_move[0].x = (player.pos.x - last.pos.x) / 3;
					E_move[0].y = (player.pos.y - last.pos.y) / 3;
				}

				last.pos.x += E_move[0].x * dt;
				last.pos.y += E_move[0].y * dt;
			}

			HIT[0] = last.pos.x - player.pos.x;
			HIT[1] = last.pos.y - player.pos.y;
			HIT[2] = last.r + player.r;

			if (HIT[0] * HIT[0] + HIT[1] * HIT[1] < HIT[2] * HIT[2])
			{
				if (sub_W != 1)
					player.life--;
			}
		}

		//弾の挙動

		for (int i = 0; i < 30; i++)
		{
			if (P_bullet[i].life)
			{
				P_bullet[i].pos.x += P_bullet[i].move.x;

				if (P_bullet[i].pos.x > width + P_bullet[i].r)
					P_bullet[i].life = 0;

				else
					for (int j = 0; j < 5; j++)
					{
						if (enemy[j].life)
						{
							HIT[0] = P_bullet[i].pos.x - enemy[j].pos.x;
							HIT[1] = P_bullet[i].pos.y - enemy[j].pos.y;
							HIT[2] = P_bullet[i].r + enemy[j].r;

							if (HIT[0] * HIT[0] + HIT[1] * HIT[1] < HIT[2] * HIT[2])
							{
								P_bullet[i].life--;
								enemy[j].life--;
								enemy_menber--;
								for (int k = 0; k < 20; k++)
									if (B_end[k].life == -1)
									{
										B_end[k].life = 0;
										B_end[k].pos.x = enemy[j].pos.x;
										B_end[k].pos.y = enemy[j].pos.y;
										B_end[k].r = 0;
										PlaySoundMem(music[2], DX_PLAYTYPE_BACK);
										break;
									}
								break;
							}
						}
					}

				if (boss.life)
				{
					HIT[0] = P_bullet[i].pos.x - boss.pos.x;
					HIT[1] = P_bullet[i].pos.y - boss.pos.y;
					HIT[2] = P_bullet[i].r + boss.r;

					if (HIT[0] * HIT[0] + HIT[1] * HIT[1] < HIT[2] * HIT[2])
					{
						P_bullet[i].life--;
						boss.life--;

						if (boss.life == 0)
						{
							boss_menber--;

							for (int k = 0; k < 20; k++)
								if (B_end[k].life == -1)
								{
									B_end[k].life = 0;
									B_end[k].pos.x = boss.pos.x;
									B_end[k].pos.y = boss.pos.y;
									B_end[k].r = 0;
									PlaySoundMem(music[2], DX_PLAYTYPE_BACK);
									break;
								}
						}
					}
				}

				if (last.life)
				{
					HIT[0] = P_bullet[i].pos.x - last.pos.x;
					HIT[1] = P_bullet[i].pos.y - last.pos.y;
					HIT[2] = P_bullet[i].r + last.r;

					if (HIT[0] * HIT[0] + HIT[1] * HIT[1] < HIT[2] * HIT[2])
					{
						P_bullet[i].life--;
						last.life--;

						if (last.life == 0)
							for (int k = 0; k < 20; k++)
								if (B_end[k].life == -1)
								{
									B_end[k].life = 0;
									B_end[k].pos.x = last.pos.x;
									B_end[k].pos.y = last.pos.y;
									B_end[k].r = 0;
									PlaySoundMem(music[2], DX_PLAYTYPE_BACK);
									break;
								}
					}
				}
			}

			if (E_bullet[i].life) {
				E_bullet[i].pos.x += E_bullet[i].move.x;
				E_bullet[i].pos.y += E_bullet[i].move.y;

				if (E_bullet[i].pos.x < -E_bullet[i].r || E_bullet[i].pos.x > width + E_bullet[i].r
					|| E_bullet[i].pos.y < -E_bullet[i].r || E_bullet[i].pos.y >height + E_bullet[i].r)
					E_bullet[i].life = 0;

				else
				{
					HIT[0] = E_bullet[i].pos.x - player.pos.x;
					HIT[1] = E_bullet[i].pos.y - player.pos.y;
					HIT[2] = E_bullet[i].r + player.r;

					if (HIT[0] * HIT[0] + HIT[1] * HIT[1] < HIT[2] * HIT[2])
					{
						if (sub_W != 1)
							player.life--;
						E_bullet[i].life--;
					}
				}
			}
		}

		if (sub_W == 2)
		{
			for (int j = 0; j < 5; j++)
			{
				if (enemy[j].life && player.pos.x + 5 <= enemy[j].pos.x + enemy[j].r)
				{
					if (player.pos.y >= enemy[j].pos.y - enemy[j].r && player.pos.y <= enemy[j].pos.y + enemy[j].r)
					{
						enemy[j].life--;
						enemy_menber--;
						for (int k = 0; k < 20; k++)
							if (B_end[k].life == -1)
							{
								B_end[k].life = 0;
								B_end[k].pos.x = enemy[j].pos.x;
								B_end[k].pos.y = enemy[j].pos.y;
								B_end[k].r = 0;
								PlaySoundMem(music[2], DX_PLAYTYPE_BACK);
								break;
							}
					}
				}
			}

			if (boss.life && player.pos.x + 5 <= boss.pos.x + boss.r)
			{
				if (player.pos.y >= boss.pos.y - boss.r && player.pos.y <= boss.pos.y + boss.r)
				{
					boss.life--;

					if (boss.life == 0)
					{
						boss_menber--;

						for (int k = 0; k < 20; k++)
							if (B_end[k].life == -1)
							{
								B_end[k].life = 0;
								B_end[k].pos.x = boss.pos.x;
								B_end[k].pos.y = boss.pos.y;
								B_end[k].r = 0;
								PlaySoundMem(music[2], DX_PLAYTYPE_BACK);
								break;
							}
					}
				}
			}

			if (last.life && player.pos.x + 5 <= last.pos.x + last.r)
			{
				if (player.pos.y >= last.pos.y - last.r && player.pos.y <= last.pos.y + last.r)
				{
					last.life--;

					if (last.life == 0)
						for (int k = 0; k < 20; k++)
							if (B_end[k].life == -1)
							{
								B_end[k].life = 0;
								B_end[k].pos.x = last.pos.x;
								B_end[k].pos.y = last.pos.y;
								B_end[k].r = 0;
								PlaySoundMem(music[2], DX_PLAYTYPE_BACK);
								break;
							}
				}
			}
		}

		if (back > 0 && back <= 640)
			back = back - 4;

		if (player.life <= 0 && end_flag == 0)
		{
			end_flag--;
			sub_W = 1;
			for (int k = 0; k < 20; k++)
				if (B_end[k].life == -1)
				{
					B_end[k].life = 0;
					B_end[k].pos.x = player.pos.x;
					B_end[k].pos.y = player.pos.y;
					B_end[k].r = 0;
					PlaySoundMem(music[2], DX_PLAYTYPE_BACK);
					break;
				}
			end_rool = 1;
		}

		else if (last.life <= 0 && end_flag == 0) {
			sub_W = 1;
			end_flag++;
			end_rool = 1;
			for (int j = 0; j < 5; j++)
				if (enemy[j].life)
				{
					enemy[j].life--;

					for (int k = 0; k < 20; k++)
						if (B_end[k].life == -1)
						{
							B_end[k].life = 0;
							B_end[k].pos.x = enemy[j].pos.x;
							B_end[k].pos.y = enemy[j].pos.y;
							B_end[k].r = 0;

							PlaySoundMem(music[2], DX_PLAYTYPE_BACK);
							break;
						}
				}
		}

		if (end_rool != 0)
			end_rool++;

		if (end_rool == 200)
			break;

		R++; G++; B++;
		if (R == 255)R = 15;
		if (G == 255)G = 15;
		if (B == 255)B = 15;
		color[0] = GetColor(R, G, B);//変動

		destX[0] = 0;
		srcX[0] = scrollCount % mapW;
		srcL[0] = mapW - srcX[0];
		if (srcL[0] > width)
			srcL[0] = width;
		destX[1] = srcL[0];
		srcX[1] = 0;
		srcL[1] = width - srcL[0];

		for (int i = 0; i < 20; i++)
		{
			if (B_end[i].life >= 0) {
				
				if (B_end[i].r == 6)
				{
					B_end[i].r = 0;
					B_end[i].life++; 

					// エフェクトの再生が完了したら消滅
					if (B_end[i].life == 20)
						B_end[i].life = -1;
				}
				B_end[i].r++;
			}
		}

		for (int i = 0; i < 256; i++)
			keyBufOld[i] = keyBuf[i];

		// 描画系の処理

		DrawRectGraph(destX[0], 0, srcX[0], 0, srcL[0], height, map, FALSE, FALSE);
		DrawRectGraph(destX[1], 0, srcX[1], 0, srcL[1], height, map, FALSE, FALSE);

		DrawBox(back, 0, 642, 480, color[0], TRUE);

		for (int i = 0; i < 20; i++)
			if (B_end[i].life != -1)
				DrawGraph((int)B_end[i].pos.x - 96,
					(int)B_end[i].pos.y - 96, bomb[B_end[i].life], TRUE);

		if (visible > 0)
		{
			for (int i = 0; i < 30; i++) {//弾
				if (P_bullet[i].life)
					DrawCircle((int)P_bullet[i].pos.x, (int)P_bullet[i].pos.y, P_bullet[i].r, color[4], vis);
				if (E_bullet[i].life)
					DrawCircle((int)E_bullet[i].pos.x, (int)E_bullet[i].pos.y, E_bullet[i].r, color[5], vis);
			}

			for (int i = 0; i < 5; i++)
				if (enemy[i].life > 0)
					DrawCircle((int)enemy[i].pos.x, (int)enemy[i].pos.y, enemy[i].r, color[3], vis);

			if (boss.life > 0)
				DrawCircle((int)boss.pos.x, (int)boss.pos.y, boss.r, color[3], vis);

			if (last.life > 0)
				DrawCircle((int)last.pos.x, (int)last.pos.y, last.r, color[3], vis);

			if (player.life > 0)
				DrawCircle((int)player.pos.x, (int)player.pos.y, player.r, color[1], vis);
		}

		for (int i = 0; i < 30; i++) {//弾
			if (P_bullet[i].life)
				DrawExtendGraph((int)P_bullet[i].pos.x - P_bullet[i].r - 2, (int)P_bullet[i].pos.y - P_bullet[i].r - 2,
					(int)P_bullet[i].pos.x + P_bullet[i].r + 2, (int)P_bullet[i].pos.y + P_bullet[i].r + 2, I_P_bu, TRUE);
			if (E_bullet[i].life)
				DrawExtendGraph((int)E_bullet[i].pos.x - E_bullet[i].r - 2, (int)E_bullet[i].pos.y - E_bullet[i].r - 2,
					(int)E_bullet[i].pos.x + E_bullet[i].r + 2, (int)E_bullet[i].pos.y + E_bullet[i].r + 2, I_E_bu, TRUE);
		}

		for (int i = 0; i < 5; i++)
			if (enemy[i].life > 0)
				DrawExtendGraph((int)enemy[i].pos.x - enemy[i].r - 1, (int)enemy[i].pos.y - enemy[i].r - 1,
					(int)enemy[i].pos.x + enemy[i].r + 1, (int)enemy[i].pos.y + enemy[i].r + 1, I_enemy, TRUE);

		if (boss.life > 0)
			DrawExtendGraph((int)boss.pos.x - boss.r, (int)boss.pos.y - boss.r,
				(int)boss.pos.x + boss.r, (int)boss.pos.y + boss.r, I_boss, TRUE);

		if (last.life > 0)
			DrawExtendGraph((int)last.pos.x - last.r, (int)last.pos.y - last.r,
				(int)last.pos.x + last.r, (int)last.pos.y + last.r, I_last, TRUE);

		if (player.life > 0)
			DrawExtendGraph((int)player.pos.x - player.r, (int)player.pos.y - player.r,
				(int)player.pos.x + player.r, (int)player.pos.y + player.r, I_player, TRUE);

		DrawFormatString(30, 30, color[1], "LIFE：%d", player.life);

		for (int i = 0; player.life > 0 && i < 2 && sub_W == 1; i++)
			DrawCircle((int)player.pos.x, (int)player.pos.y, player.r - i, color[0], FALSE);

		if (sub_W == 2)
		{
			DrawBox((int)player.pos.x + 5, (int)player.pos.y + 2, 640, (int)player.pos.y - 2, color[4], TRUE);
			DrawBox((int)player.pos.x + 5, (int)player.pos.y + 1, 640, (int)player.pos.y - 1, color[0], TRUE);
		}
		ScreenFlip();

		scrollCount++;
	}
	StopSoundMem(music[0]);

	DrawBox(120, 120, 520, 360, color[1], TRUE);
	DrawBox(210, 180, 410, 300, color[5], TRUE);

	if (end_flag == -1 && Esc == 0)
		DrawFormatString(270, 210, color[1], "GAME_OVER");

	else if (end_flag == 1 && Esc == 0)
		DrawFormatString(270, 210, color[1], "GAME_CLEAR");


	if (player.life > 6)
		DrawFormatString(272, 228, color[1], "SCORE:極上");

	else if (player.life > 4)
		DrawFormatString(272, 228, color[1], "SCORE:上");

	else if (player.life > 2)
		DrawFormatString(272, 228, color[1], "SCORE:中");

	else if (player.life > 0)
		DrawFormatString(272, 228, color[1], "SCORE:並");

	else
		DrawFormatString(272, 228, color[1], "SCORE:等外");

	DrawFormatString(215, 260, color[1], "適当なキーを押して終了");

	if (Esc == 0)
	{
		ScreenFlip();
		WaitKey();
	}

	DxLib_End();
	return 0;
}