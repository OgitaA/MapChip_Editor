#pragma once

struct Size_Save {

	int size = 72;


	// シリアライズに対応させるためのメンバ関数を定義する
	template <class Archive>
	void SIV3D_SERIALIZE(Archive& archive)
	{
		archive(size);
	}
};
