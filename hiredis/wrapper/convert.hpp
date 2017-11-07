#ifndef M_CONVERT_INCLUDE
#define M_CONVERT_INCLUDE

#include <sstream>
#include <string.h>

void convert_to_array(const char* src, char* dst, unsigned int dst_len)
{
	if (dst_len > strlen(src))
		dst_len = strlen(src);
	memcpy(dst, src, dst_len);
}
void convert_to_array(const char* src, unsigned int src_len, char* dst, unsigned int dst_len)
{
	if (dst_len > src_len)
		dst_len = src_len;
	memcpy(dst, src, dst_len);
}
void convert_to_array(const std::string& src, char* dst, unsigned int dst_len)
{
	if (dst_len > src.size())
		dst_len = src.size();
	memcpy(dst, src.c_str(), dst_len);
}
template<int N>
void convert_to_array(const char(&src)[N], char* dst, unsigned int dst_len)
{
	if (dst_len > N)
		dst_len = N;
	memcpy(dst, src(), dst_len);
}
template<typename T>
void convert_to_array(const T& src, char* dst, unsigned int dst_len)
{
	std::ostringstream oss;
	oss << src;
	convert_to_array(oss.str(), dst, dst_len);
}

template<typename T1, typename T2>
void convert_to_t(T1&src, T2& dst)
{
	std::ostringstream oss;
	oss << src;
	convert_to_t(oss.str(), dst);
}
template<typename T>
void convert_to_t(T&src, std::string& dst)
{
	std::ostringstream oss;
	oss << src;
	dst = oss.str();
}
template<typename T>
void convert_to_t(const char* src, T& dst)
{
	std::istringstream iss(std::string(src));
	iss >> dst;
}
void convert_to_t(const char* src, std::string& dst)
{
	dst.clear();
	dst.append(src);
}
template<typename T>
void convert_to_t(const char* src, unsigned int src_len, T& dst)
{
	std::istringstream iss(std::string(src,src_len));
	iss >> dst;
}
void convert_to_t(const char* src, unsigned int src_len, std::string& dst)
{
	dst.clear();
	dst.append(src, src_len);
}
template<typename T>
void convert_to_t(const std::string& src, T& dst)
{
	std::istringstream iss(src);
	iss >> dst;
}
void convert_to_t(const std::string& src, std::string& dst)
{
	dst = src;
}
template<int N,typename T>
void convert_to_t(const char(&src)[N], T& dst)
{
	std::istringstream iss(std::string(src, N));
	iss >> dst;
}
template<int N>
void convert_to_t(const char(&src)[N], std::string& dst)
{
	dst.clear();
	dst.append(src, N);
}





#endif