#ifndef M_CONVERT_INCLUDE
#define M_CONVERT_INCLUDE

#include <sstream>

template<int N>
void convert_to(const char(&src)[N], std::string& dst)
{
	dst.append(src, N);
}
void convert_to(const char*src, std::string& dst)
{
	dst.append(src);
}
void convert_to(const std::string& src, std::string& dst)
{

}
template<typename T>
void convert_to(const char*src, T&dst)
{

}

void convert_to(const char*src, unsigned int len, std::string& dst);
template<typename T>
void convert_to(const char*src, unsigned int len, T& dst);
template<int N,typename T>
void convert_to(const char(&src)[N], T& dst);

template<typename T>
void convert_to(const T& src, std::string& dst);
template<typename T>
//void convert_to()


void convert_to_string(const char* src,std::string& dst)
{
	dst.append(src);
}
void convert_to_string(const char* src, unsigned int len, std::string& dst)
{
	dst.append(src, len);
}
template<typename T>
void convert_to_string(const T& src, std::string& dst)
{
	std::ostringstream oss;
	oss << src;
	dst = oss.str();
}
void convert_to_array(const char* src, unsigned int src_len, char* dst, unsigned int dst_len)
{
	if (dst_len > src_len)
		dst_len = src_len;
	memcpy(dst, src, dst_len);
}


#endif