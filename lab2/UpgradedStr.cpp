#include <random>
#include <vector>
#include <chrono>

#include <iostream>

using namespace std;

class DemoStr
{
public:
	DemoStr() : data_{ nullptr }, length_{ 0 }, capacity_{ 0 }
	{}

	DemoStr(char const* raw_str, size_t length) : length_{ length }, capacity_{ length }
	{
		allocate(length);
		copy_from(raw_str, data_, length);
		reformat();
	}

	DemoStr(DemoStr const& other) : length_{ other.length_ }, capacity_{ length_ }
	{
		allocate(other.length_);
		copy_from(other.data_, data_, other.length_);
	}

	explicit DemoStr(int value)
	{
		length_ = get_value_length(value);
		allocate(length_);
		auto it = &data_[length_ - 1];
		while (value > 0)
		{
			*it = value % 10 + 0x30;
			--it;
			value /= 10;
		}
	}

	void print() const
	{
		for (size_t i = 0; i < length_; i++)
		{
			cout << data_[i];
		}
		cout << '\n';
	}

	char& operator[](size_t index)
	{
		return data_[index];
	}

	const char& operator[](size_t index) const
	{
		return data_[index];
	}

	bool equal(DemoStr const& other) const
	{
		if (length_ != other.length_)
		{
			return false;
		}

		for (size_t i = 0; i < length_; i++)
		{
			if (data_[i] != other.data_[i])
			{
				return false;
			}
		}
		return true;
	}

	void clear()
	{
		delete[] data_;
		data_ = nullptr;
		length_ = capacity_ = 0;
	}

	void read_from(istream& in)
	{
		char tmp;
		while (in >> tmp)
		{
			if (length_ + 1 >= capacity_)
				reallocate((capacity_ == 0) ? 1 : capacity_ * 2);
			data_[length_] = tmp;
			++length_;
		}
		reformat();
	}

	~DemoStr()
	{
		clear();
	}

	void shrink_to_fit()
	{
		reallocate(length_);
	}
protected:
	char* get_data() const {
		return data_;
	}

	void reallocate(size_t n)
	{
		char* old_data = data_;

		data_ = new char[n];
		capacity_ = n;

		copy_from(old_data, data_, (n > length_) ? length_ : n);

		delete[] old_data;
	}

	void reformat()
	{
		for (size_t i = 0; i < length_; i++)
		{
			data_[i] = (i % 2 == 0) ? tolower(data_[i]) : toupper(data_[i]);
		}
	}

	size_t get_value_length(int value)
	{
		size_t len = 0;
		while (value > 0)
		{
			value /= 10;
			len++;
		}
		return len;
	}

	void allocate(size_t n)
	{
		data_ = new char[n];
	}

	void copy_from(char const* src, char* dst, size_t n)
	{
		for (size_t i = 0; i < n; i++)
		{
			dst[i] = src[i];
		}
	}

	friend ostream& operator<< (ostream& out, DemoStr const& v);

	size_t get_length() const {
		return length_;
	}

	size_t get_capacity() const {
		return capacity_;
	}
	void set_length(size_t n) {
		length_ = n;
	}
private:
	char* data_ = nullptr;
	size_t length_ = 0;
	size_t capacity_ = 0;
};

ostream& operator<< (ostream& out, DemoStr const& v)
{
	for (size_t i = 0; i < v.length_; i++)
	{
		out << v.data_[i];
	}
	return out;
}

istream& operator>> (istream& in, DemoStr& v)
{
	v.clear();
	v.read_from(in);
	return in;
}

class FixedStr : public DemoStr {
public:
	FixedStr(char const* raw_str, size_t length) : DemoStr(raw_str, length)
	{}

	FixedStr() : DemoStr()
	{}

	FixedStr(FixedStr const& other) : DemoStr(other)
	{}

	FixedStr(int num) : DemoStr(num)
	{}


	void append(FixedStr str) {
		reallocate(get_length() + str.get_length());
		for (size_t i = 0; i < str.get_length(); i++)
		{
			(*this)[i + get_length()] = str[i];
		}
		set_length(get_length() + str.get_length());
	}

	vector<FixedStr> split(char c) {
		vector<FixedStr> res(0);
		size_t start_pos = 0;
		for (size_t i = 0; i < get_length(); i++)
		{
			if (tolower((*this)[i]) == c) {
				res.push_back(FixedStr(get_data() + start_pos, i - start_pos));
				/*res.push_back(create_str(start_pos, i, *this));*/
				start_pos = i + 1;
			}
		}
		res.push_back(FixedStr(get_data() + start_pos, get_length() - start_pos));
		return res;
	}
};

class TimeMeasure {
public:
	void start() {
		start_time_ = std::chrono::high_resolution_clock::now();
	}
	void stop() {
		stop_time_ = std::chrono::high_resolution_clock::now();
	}
	size_t elapsed() const {
		auto elapsed_time = stop_time_ - start_time_;
		return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time).count();
	}
private:
	std::chrono::high_resolution_clock::time_point start_time_, stop_time_;
};

class RandomGenerator {
public:
	static std::vector<char> create_sequence(size_t n, int min, int max) {
		//std::mt19937_64 engine(std::random_device{}());
		std::mt19937_64 engine(42);
		std::uniform_int_distribution<int> distr(min, max);
		std::vector<char> v(n);
		for (size_t i = 0; i < n; i++)
		{
			v[i] = distr(engine);
		}
		return v;
	}
private:

};

int main(int argc, char** argv) {
	TimeMeasure timer;
	/*TimeMeasure timer;

	timer.start();

	for (size_t i = 0; i < 10000; i++)
	{
		auto random_vec = RandomGenerator::create_sequence(10000, -10, 10);
	}

	timer.stop();

	std::cout << "ETA (x10000): " << timer.elapsed() << std::endl;*/
	char demo[] = "Hello";

	FixedStr a(demo, 5);
	FixedStr b = a;
	cout << a << ' ' << b << endl;

	for (size_t i = 0; i < 10000000; i+=10000)
	{
		int start_str_size = i;
		auto start_vec = RandomGenerator::create_sequence(start_str_size, -127, 127);
		FixedStr f(start_vec.data(), start_vec.size());
		

		/*auto random_vec = RandomGenerator::create_sequence(i, -127, 127);
		FixedStr s(random_vec.data(), random_vec.size());*/

		timer.start();
		for (size_t j = 0; j < 100; j++)
		{
			FixedStr n = f;
		}
		timer.stop();

		std::cout << i << " " << timer.elapsed() << std::endl;

	}

	cout << a << endl;

	char test[] = "MikelAiklSasha";
	FixedStr c(test, 14);
	vector<FixedStr> result(0);
	result = c.split('l');


	return 0;
}

