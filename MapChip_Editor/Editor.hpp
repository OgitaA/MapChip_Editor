#pragma once

#include"My_Button.hpp"
#include"My_Slider.hpp"

#include"Size_Save.hpp"

struct Editor {

	//１マスのサイズ
	int size = 72;

	//最大
	int size_max = 150;

	Image base_image = { size * 10,size * 10,Palette::White };
	Texture base_texture{ base_image };

	struct Object {

		Object(Image set_image, Texture set_texture, int set_x, int set_y) {
			m_image = set_image;
			m_texture = set_texture;
			m_x = set_x;
			m_y = set_y;
			ID = ID_count;
			ID_count++;
		}

		void draw(int size, float scroll) { m_texture.draw(m_x * size, m_y * size - scroll); }

		Image m_image;
		Texture m_texture;
		int m_x;
		int m_y;
		int ID;

		static int ID_count;
	};

	


	Array<Object> objects;

	int x = 0;
	int y = 0;

	//GUI
	Array<My_Slider> my_sliders;
	Array<My_Button> my_buttons;

	float scroll_y = 0;

	// 基本サイズ 30 のフォントを作成
	const Font font{ 30 };

	
	//選択中のObjectのID
	int select_ID = -1;

	Rect select_rect;

	struct Spawn_Rect {

		int x = 0;
		int y = 0;

		Rect rect;
	};

	Spawn_Rect spawn_rect;

	Size_Save size_save;
	
	float save_display = 0;

	String mode = U"select";
	

	void init() {

		FontAsset::Register(U"Button", 30, Typeface::Medium);
		FontAsset::Register(U"Big", 100, Typeface::Medium);


		//U"XバーとYバーは対称のものではない
		my_sliders.push_back(My_Slider(U"Y", 750, 100, 400, 1000));

		my_sliders.push_back(My_Slider(U"X", 750, 300, 400, 200));

		my_buttons.push_back(My_Button(U"ベース", 800, 600, 100, 100));
		my_buttons.push_back(My_Button(U"出力", 900+50, 600, 200, 100));

		my_buttons.push_back(My_Button(U"↑", 900+60, 300, 80, 80));
		my_buttons.push_back(My_Button(U"↓", 900+60, 400, 80, 80));
		my_buttons.push_back(My_Button(U"←", 800+60, 350, 80, 80));
		my_buttons.push_back(My_Button(U"→", 1000+60, 350, 80, 80));

		my_buttons.push_back(My_Button(U"サイズ決定", 1050-200, 100, 170, 100));
		my_buttons.push_back(My_Button(U"削除", 1050,100, 100, 100));


		my_buttons.push_back(My_Button(U"＋", 1050 + 0, 10, 50, 50));
		my_buttons.push_back(My_Button(U"－", 1050 + 70, 10, 50, 50));

		my_buttons.push_back(My_Button(U"選択", 800, 450+20, 100, 100));

		read_size_save();
		size = size_save.size;

	
	}

	void update() {



		//ドロップでもってくる
		if (DragDrop::HasNewFilePaths())
		{
			const Image image{ DragDrop::GetDroppedFilePaths().front().path };

			Texture texture{ image };

			objects.push_back(Object(image, texture, spawn_rect.x, spawn_rect.y));

			int plus = texture.width() / size;

			bool stop = false;

			if (10 <= spawn_rect.x + plus) {

				if (10 <= spawn_rect.y + 1) {
					stop = true;
				}
			}

			if (false == stop) {

				spawn_rect.x += plus;

				if (10 == spawn_rect.x) {
					spawn_rect.x = 0;
					spawn_rect.y++;
				}
			}
		}


		String direction = U"";

		for (auto& button : my_buttons) {

			button.update();

			if (button.get_click()) {

				if (U"出力" == button.get_name()) {
					output_image();
				}

				if (U"削除" == button.get_name()) {
					if (-1 != select_ID) {
						delete_object();
					}
				}

				if (U"ベース" == button.get_name()) {
					set_base_image();
				}

				if (U"＋" == button.get_name()) {

					size++;
					if (size_max < size) {
						size = size_max;
					}

				}
				else if (U"－" == button.get_name()) {
					size--;
					if (size < 1) {
						size = 1;
					}
				}

				if (U"サイズ決定" == button.get_name()) {
					size_save.size = size;
					write_size_save();
					save_display = 1;
					base_image = {size_t(size)*10,size_t(size)*10,Palette::White};
					base_texture = Texture(base_image);
				}

				if (U"選択" == button.get_name()) {
					button.change_name(U"生成");
					mode = U"spawn";
				}
				else if (U"生成" == button.get_name()) {
					button.change_name(U"選択");
					mode = U"select";
				}




				if (U"↑" == button.get_name()) {
					direction = U"↑";
				}
				else if (U"↓" == button.get_name()) {
					direction = U"↓";
				}
				else if (U"←" == button.get_name()) {
					direction = U"←";
				}
				else if (U"→" == button.get_name()) {
					direction = U"→";
				}

			}
		}

		if (U"select" == mode) {

			if (-1 != select_ID) {
				if (U"" != direction) {

					for (auto& object : objects) {

						if (object.ID == select_ID) {

							if (U"↑" == direction) {
								object.m_y--;

								if (object.m_y < 0) {
									object.m_y = 0;
								}
							}
							else if (U"↓" == direction) {
								object.m_y++;

								if (9 < object.m_y) {
									object.m_y = 9;
								}
							}
							else if (U"←" == direction) {
								object.m_x--;
								if (object.m_x < 0) {
									object.m_x = 0;
								}
							}
							else if (U"→" == direction) {
								object.m_x++;

								if (9 < object.m_x) {
									object.m_x = 9;
								}
							}
						}
					}
				}
			}

		}
		else if (U"spawn" == mode) {

		

			if (U"↑" == direction) {
				
				spawn_rect.y--;

				if (spawn_rect.y < 0) {
					spawn_rect.y = 0;
				}
			}
			else if (U"↓" == direction) {
				spawn_rect.y++;

				if (9 < spawn_rect.y) {
					spawn_rect.y = 9;
				}
			}
			else if (U"←" == direction) {

				spawn_rect.x--;

				if (spawn_rect.x < 0) {
					spawn_rect.x = 0;
				}
			}
			else if (U"→" == direction) {
				spawn_rect.x++;

				if (9 < spawn_rect.x) {
					spawn_rect.x = 9;
				}
			}
		}

		for (auto& slider : my_sliders) {

			if (slider.get_type() == U"Y") {
				scroll_y = slider.update();
			}
		}


		//Objectを選択
		for (auto& object : objects) {

			Rect rect(object.m_x * size, object.m_y * size, object.m_texture.width(), object.m_texture.height());


			if (MouseL.down()) {

				Vec2 point = { Cursor::PosF().x,Cursor::PosF().y + scroll_y };

				if (rect.intersects(point)) {
					select_ID = object.ID;
					break;
				}

			}

		}

		//Select_Rect

		if (-1 != select_ID) {

			for (auto& object : objects) {

				if (object.ID == select_ID) {
					select_rect = Rect(object.m_x * size, object.m_y * size, object.m_texture.width(), object.m_texture.height());
					break;
				}
			}
		}


		//Spawn_Rect
		spawn_rect.rect = Rect(spawn_rect.x * size, spawn_rect.y * size, size, size);

		














	}

	void draw() {

		//Draw(編集確認用)
		{


			base_texture.draw(0, 0 - scroll_y);

			for (auto& object : objects) {
				object.draw(size, scroll_y);
			}

		}

		font(U"タイルのサイズ::" + Format(size)).draw(850-100, 20);


		for (auto& slider : my_sliders) {
			if (slider.get_type() == U"Y") {
				slider.draw();
			}
		}


		for (auto& button : my_buttons) {


			button.draw();
		}

		if (-1 != select_ID) {

			select_rect.movedBy(0, -scroll_y).drawFrame(5, Palette::Red);
		}

		spawn_rect.rect.movedBy(0, -scroll_y).drawFrame(5, Palette::Green);


		if (0<save_display) {
			save_display -= Scene::DeltaTime();
		}

		FontAsset(U"Big")(U"SAVE").drawAt(1200 / 2, 720 / 2, ColorF(1, 0, 0, save_display));
		
	}

	void set_base_image() {

		base_image = Dialog::OpenImage();
		base_texture = Texture(base_image);

	}

	void output_image() {

		for (auto& object : objects) {

			int write_x = object.m_x * size;
			int write_y = object.m_y * size;
			Image write_image = object.m_image;

			write_image.overwrite(base_image, write_x, write_y);
		}


		//base_texture = Texture{ base_image };

		base_image.saveWithDialog();
	}

	void delete_object() {

		objects.remove_if([&](Object object) {

			if (object.ID == select_ID) {
				return true;
			}
	        	return false;

			});

		select_ID = -1;
	}

	void read_size_save() {

		// バイナリファイルをオープン
		Deserializer<BinaryReader> reader{ U"size_save.bin" };

		if (not reader) // もしオープンに失敗したら
		{
			throw Error{ U"Failed to open `tutorial4.bin`" };
		}

		// バイナリファイルからシリアライズ対応型のデータを読み込む
		// （Array は自動でリサイズが行われる）
		reader(size_save);
	}

	void write_size_save() {

		// バイナリファイルをオープン
		Serializer<BinaryWriter> writer{ U"size_save.bin" };

		if (not writer) // もしオープンに失敗したら
		{
			throw Error{ U"Failed to open `tutorial4.bin`" };
		}

		// シリアライズに対応したデータを記録
		writer(size_save);
	}
	
};

