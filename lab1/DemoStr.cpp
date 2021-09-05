#include <iostream>

using namespace std;

class DemoStr
{
public:
	DemoStr() : data_{nullptr}, length_{0}, capacity_{0}
	{}

	DemoStr(char const* raw_str, size_t length) : length_{length}, capacity_{length}
	{
		allocate(length);
		copy_from(raw_str, length);
		reformat();
	}

	DemoStr(DemoStr const& other) : length_{other.length_}, capacity_{length_}
	{
		allocate(other.length_);
		copy_from(other.data_, other.length_);
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

	//index < length_ - 1
	char at(size_t index) const
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
private:
	void reallocate(size_t n)
	{
		char* old_data = data_;

		data_ = new char[n];
		capacity_ = n;

		copy_from(old_data, (n > length_) ? length_ : n);

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

	void copy_from(char const* data, size_t n)
	{
		for (size_t i = 0; i < n; i++)
		{
			data_[i] = data[i];
		}
	}

	friend ostream& operator<< (ostream& out, DemoStr const& v);

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

int main(int argc, char** argv)
{
	char demo[] = "Hello, class";
	DemoStr str(demo, sizeof(demo) - 1);

	cout << str << '\n';
	str.clear();
	cout << str << '\n';

	cin >> str;
	str.shrink_to_fit();
	cout << "\nReaded data: " << str;

	return 0;
}
