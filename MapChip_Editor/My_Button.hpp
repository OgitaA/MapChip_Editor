#pragma once

#include<Siv3D.hpp>

class My_Button {

public:

	My_Button() {}
	My_Button(String set_name, int x, int y, int w, int h) {
		m_name = set_name;
		m_rect = RectF(x, y, w, h);
	}

	void update() {

		click = false;
		press = false;

		if (m_rect.leftClicked()) {
			click = true;
		}

		if (m_rect.leftPressed()) {
			press = true;
		}
	}

	void draw()const {

		RectF rect = m_rect;

		rect.rounded(3).draw(Palette::White);

		ColorF color = Palette::Black;

		if (U"選択" == m_name) {
			color = Palette::Red;
		}
		else if (U"生成" == m_name) {
			color = Palette::Green;
		}

		FontAsset(U"Button")(m_name).drawAt({ rect.x + rect.w / 2,rect.y + rect.h / 2 }, color);

	}

	bool get_click() { return click; }

	bool get_press() { return press; }

	String get_name() { return m_name; }


	//特殊な処理
	void change_name(String set_name) {
		m_name = set_name;
	}

private:

	String m_name;

	RectF m_rect;

	bool click = false;

	bool press = false;
};
