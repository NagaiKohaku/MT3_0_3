#include <Novice.h>
#include <assert.h>

const char kWindowTitle[] = "LE2B_17_ナガイ_コハク_MT3_0_3 4x4行列の拡縮・平行移動";

//4x4行列
struct Matrix4x4 {
	float m[4][4];
};

//3次元ベクトル
struct Vector3 {
	float x, y, z;
};

//平行移動行列を作成する関数
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

//拡大縮小行列を作成する関数
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

//座標変換をする関数
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

//3次元ベクトルの各値の表示
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);

//4x4行列の表示
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);

//表示幅:横
static const int kColumnWidth = 60;

//表示幅:縦
static const int kRowHeight = 20;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//平行移動させる量
	Vector3 translate{ 4.1f,2.6f,0.8f };

	//回転させる量
	Vector3 scale{ 1.5f,5.2f,7.3f };

	//初期位置
	Vector3 point{ 2.3f,3.8f,1.4f };

	//座標変換する4x4行列
	Matrix4x4 transformMatrix = {
		1.0f,2.0f,3.0f,4.0f,
		3.0f,1.0f,1.0f,2.0f,
		1.0f,4.0f,2.0f,3.0f,
		2.0f,2.0f,1.0f,3.0f
	};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		//平行移動行列
		Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

		//拡大縮小行列
		Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);

		//座標変換
		Vector3 transformed = Transform(point, transformMatrix);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		//平行移動行列の内容表示
		VectorScreenPrintf(0, 0, transformed, "transformed");

		//拡大縮小行列の内容表示
		MatrixScreenPrintf(0, kRowHeight, translateMatrix, "translateMatrix");
		
		//座標変換した行列の内容表示
		MatrixScreenPrintf(0, kRowHeight * 6, scaleMatrix, "scaleMatrix");

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

//平行移動行列を作成する関数
Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {

	Matrix4x4 result;

	result.m[0][0] = 1;
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[0][3] = 0;
	result.m[1][0] = 0;
	result.m[1][1] = 1;
	result.m[1][2] = 0;
	result.m[1][3] = 0;
	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = 1;
	result.m[2][3] = 0;
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	result.m[3][3] = 1;

	return result;
}

//拡大縮小行列を作成する関数
Matrix4x4 MakeScaleMatrix(const Vector3& scale) {

	Matrix4x4 result;

	result.m[0][0] = scale.x;
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[0][3] = 0;
	result.m[1][0] = 0;
	result.m[1][1] = scale.y;
	result.m[1][2] = 0;
	result.m[1][3] = 0;
	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = scale.z;
	result.m[2][3] = 0;
	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = 0;
	result.m[3][3] = 1;

	return result;
}

//座標変換をする関数
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {

	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];

	assert(w != 0.0f);

	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}

//3次元ベクトルの各値の表示
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {

	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}

//4x4行列の表示
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {

	//計算方法の表示
	Novice::ScreenPrintf(x, y, label);

	//行列の内容の表示
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(
				x + column * kColumnWidth, y + (row + 1) * kRowHeight, "%6.02f", matrix.m[row][column]
			);
		}
	}
}