#pragma once
#include<Siv3D.hpp>

class My_Slider {

public:


	My_Slider() {}
	My_Slider(String set_type, float set_flame_x, float set_flame_y, float set_flame_max, float set_v) :
		m_type(set_type),
		m_flame(Vec2{ set_flame_x,set_flame_y }),
		m_pos(-1, -1),
		m_old_pos(m_pos),
		m_flame_max(set_flame_max)
	{
		if (U"X" == m_type) {
			//m_x = set_flame_x + 10;
			//カーソル初期位置
			//m_x = 1920 / 2 - m_wide / 2;
			m_x = m_flame.x;
			m_y = m_flame.y + m_flame_size / 2 - m_height / 2;
			m_v = set_v;
		}
		else if (U"Y" == m_type) {
			//カーソル初期位置
			m_x = m_flame.x + m_flame_size / 2 - m_wide / 2;
			//m_y = 1080 / 2 - m_height/2;
			m_y = m_flame.y;
			m_v = set_v;
		}


		//フレーム生成
		{
			if (U"X" == m_type) {
				m_flame_rect = RectF(m_flame.x - m_blank_size, m_flame.y, m_flame_max - m_flame.x + (m_blank_size * 2) + m_wide, m_flame_size);
			}
			else if (U"Y" == m_type) {
				m_flame_rect = RectF(m_flame.x, m_flame.y - m_blank_size, m_flame_size, m_flame_max - m_flame.y + (m_blank_size * 2) + m_height);
			}

		}

	}

	int update() {

		//Click
		m_click = false;

		//四角カーソル
		RectF cur_rect(m_x, m_y, m_wide, m_height);

		/*
		if (cur_rect.leftPressed()) {
			m_click = true;
		}*/

		if (m_flame_rect.leftPressed()) {
			m_click = true;
		}


		if (0 == state) {

			if (cur_rect.leftPressed()) {
				m_pos = Cursor::PosF();
				m_old_pos = Cursor::PosF();

				state++;
			}

		}
		else if (1 == state) {

			if (MouseL.pressed()) {

				m_old_pos = m_pos;
				m_pos = Cursor::PosF();


				//四角カーソルを移動
				if (U"X" == m_type) {
					m_x += m_pos.x - m_old_pos.x;
				}
				else if (U"Y" == m_type) {
					m_y += m_pos.y - m_old_pos.y;
				}
			}

		}

		if (1 == state) {
			m_click = true;
		}


		//離されたら
		if (not MouseL.pressed()) {
			state = 0;
		}


		//制限

		if (U"X" == m_type) {

			if (m_x < m_flame.x) {
				m_x = m_flame.x;
			}
			else if (m_x > m_flame_max) {
				m_x = m_flame_max;
			}

		}
		else if (U"Y" == m_type) {

			if (m_y < m_flame.y) {
				m_y = m_flame.y;
			}
			else if (m_y > m_flame_max) {
				m_y = m_flame_max;
			}
		}

		//現在の値
		int v = 0;

		if (U"X" == m_type) {

			v = m_v * ((m_x - m_flame.x) / (m_flame_max - m_flame.x));

			//Print << U"m_x::" << m_x;
			//Print << U"m_flame.x::" << m_flame.x;
			//Print << U"per::" << ((m_x - m_flame.x) / (m_flame_max - m_flame.x));
		}
		else if (U"Y" == m_type) {

			v = m_v * ((m_y - m_flame.y) / (m_flame_max - m_flame.y));
		}


		return v;
	}

	void draw()const {

		if (U"X" == m_type) {

			//blank
			//Rect rect(m_flame.x - m_blank_size, m_flame.y, m_flame_max + (m_blank_size * 2) + m_wide, m_flame_size);
			//rect.draw(Palette::White);
			m_flame_rect.draw(Palette::White);

			//装飾(灰色)
			Rect gray_rect(m_flame.x, m_flame.y + 20, m_flame_max - m_flame.x + m_wide, m_flame_size - 40);
			gray_rect.draw(Palette::Gray);

			//Cur
			RectF cur_rect(m_x, m_y, m_wide, m_height);

			if (0 == state) {
				cur_rect.draw(Palette::Orange);
			}
			else if (1 == state) {
				cur_rect.draw(Palette::Red);
			}
		}
		else if (U"Y" == m_type) {

			//blank
			//Rect rect(m_flame.x, m_flame.y - m_blank_size, m_flame_size, m_flame_max + (m_blank_size * 2) + m_height);
			//rect.draw(Palette::White);
			m_flame_rect.draw(Palette::White);

			//装飾
			Rect gray_rect(m_flame.x + 20, m_flame.y, m_flame_size - 40, m_flame_max - m_flame.y + m_height);
			gray_rect.draw(Palette::Gray);

			//Cur
			RectF cur_rect(m_x, m_y, m_wide, m_height);
			if (0 == state) {
				cur_rect.draw(Palette::Orange);
			}
			else if (1 == state) {
				cur_rect.draw(Palette::Red);
			}
		}

	}



	bool get_click() { return m_click; }

	RectF get_rect() { return RectF(); }

	String get_type() { return m_type; }
	void set_v(int set_v) {

		if (U"X" == m_type) {
			m_v = set_v - 1920;
		}
		else if (U"Y" == m_type) {
			m_v = set_v - 1080;
		}
	}

	

private:

	String m_type;

	//扱う値
	float m_v;

	//通常カーソル
	Vec2 m_pos;
	Vec2 m_old_pos;




	//四角カーソル
	RectF m_cur_rect;//未使用


	float m_x;
	float m_y;

	const int m_wide = 40;
	const int m_height = 40;

	//flame
	RectF m_flame_rect;


	Vec2 m_flame;

	float m_flame_max;//フレームの長さ

	const int m_flame_size = 50;


	//余白
	const int m_blank_size = 10;

	//表示調整用

	//クリック判定
	bool m_click = false;

	//State
	int state = 0;


};
