#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

class Image {
public:
	Image(vector<vector<vector<int>>> raw, size_t width, size_t height, size_t depth) : width{ width }, height{ height }, depth{ depth },
		Data_{ raw }
	{}
	Image() : width{ 0 }, height{ 0 }, depth{ 0 },
		Data_{ vector<vector<vector<int>>>(0, vector<vector<int>>(0, vector<int>(0, 0))) }
	{}
	Image(size_t width, size_t height, size_t depth) : width{width}, height{height}, depth{depth},
		Data_{ vector<vector<vector<int>>>(depth, vector<vector<int>>(width, vector<int>(height, 0))) }
	{}
	Image(Image const & other) : width{other.width}, height{other.height}, depth{other.depth}, Data_{other.Data_}
	{}
	size_t get_width() const {
		return width;
	}
	size_t get_height() const {
		return height;
	}
	size_t get_depth() const {
		return depth;
	}
	void to_gs() {
		int tmp = 0;
		//vector<vector<vector<int>>> new_Data(vector<vector<vector<int>>>(1, vector<vector<int>>(width, vector<int>(height, 0))));
		for (size_t i = 0; i < height; i++)
		{
			for (size_t j = 0; j < width; j++)
			{
				tmp = 0;
				for (size_t k = 0; k < depth; k++)
				{
					tmp += Data_[k][j][i];
				}
				tmp /= 3;
				Data_[0][j][i] = tmp;
			}
		}
		Data_.resize(1);
		depth = 1;
	}
	int at(int x, int y, int d) const {
		return Data_[d][x][y];
	}
	void print() const {
		for (size_t i = 0; i < height; i++)
		{
			for (size_t j = 0; j < width; j++)
			{
				cout << "{";
				for (size_t k = 0; k < depth; k++)
				{
					cout << Data_[k][j][i] << ",";
				}
				cout << "}" << ' ';
			}
			cout << '\n';
		}
	}
	void to_bmp() {
		FILE* f;
		unsigned char* img = NULL;
		int filesize = 54 + 3 * width * height;  //w is your image width, h is image height, both int

		img = (unsigned char*)malloc(3 * width * height);
		memset(img, 0, 3 * width * height);

		int x, y;

		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				x = i; y = (height - 1) - j;
				img[(x + y * width) * 3 + 2] = (unsigned char)(Data_[0][i][j]);
				img[(x + y * width) * 3 + 1] = (unsigned char)(Data_[1][i][j]);
				img[(x + y * width) * 3 + 0] = (unsigned char)(Data_[2][i][j]);
			}
		}

		unsigned char bmpfileheader[14] = { 'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0 };
		unsigned char bmpinfoheader[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0 };
		unsigned char bmppad[3] = { 0,0,0 };

		bmpfileheader[2] = (unsigned char)(filesize);
		bmpfileheader[3] = (unsigned char)(filesize >> 8);
		bmpfileheader[4] = (unsigned char)(filesize >> 16);
		bmpfileheader[5] = (unsigned char)(filesize >> 24);

		bmpinfoheader[4] = (unsigned char)(width);
		bmpinfoheader[5] = (unsigned char)(width >> 8);
		bmpinfoheader[6] = (unsigned char)(width >> 16);
		bmpinfoheader[7] = (unsigned char)(width >> 24);
		bmpinfoheader[8] = (unsigned char)(height);
		bmpinfoheader[9] = (unsigned char)(height >> 8);
		bmpinfoheader[10] = (unsigned char)(height >> 16);
		bmpinfoheader[11] = (unsigned char)(height >> 24);

		f = fopen("img.bmp", "wb");
		fwrite(bmpfileheader, 1, 14, f);
		fwrite(bmpinfoheader, 1, 40, f);
		for (int i = 0; i < height; i++)
		{
			fwrite(img + (width * (height - i - 1) * 3), 3, width, f);
			fwrite(bmppad, 1, (4 - (width * 3) % 4) % 4, f);
		}

		free(img);
		fclose(f);
	}
private:
	vector<vector<vector<int>>> Data_;
	size_t width, height, depth;
};

int main(int argc, char** argv)
{
	vector<vector<vector<int>>> a(3, vector<vector<int>>(8, vector<int>(8, 3)));
	Image b(a, 8, 8, 3);
	Image c = b;

	b.print();
	b.to_gs();
	b.print();

	c.print();
	c.to_bmp();

	return 0;
}
