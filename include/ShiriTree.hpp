#ifndef YSL_SHIRITREE_HPP_
#define YSL_SHIRITREE_HPP_

#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <codecvt>

namespace YSL{
	class ShiriTree {
		class shiritree_word_{
			private:
				std::wstring wstr_ = L"       ";
				bool isUse_ = false;
				wchar_t first_letter_ = L' ';
				wchar_t last_letter_ = L' ';
				wchar_t to50Letters(wchar_t ch){
					constexpr wchar_t from_kata[30] = L"ガギグゲゴザジズゼゾダヂヅデドバビブベボパピプペポィャュョ";
					constexpr wchar_t to_kata[30]   = L"カキクケコサシスセソタチツテトハヒフヘホハヒフヘホイヤユヨ";
					for(int i=0; i<30; ++i) if(ch == from_kata[i]) return to_kata[i];
					constexpr wchar_t from_hira[30] = L"がぎぐげござじずぜぞだぢづでどばびぶべぼぱぴぷぺぽぃゃゅょ";
					constexpr wchar_t to_hira[30]   = L"かきくけこさしすせそだちつてとはひふへほはひふへほいやゆよ";
					for(int i=0; i<30; ++i) if(ch == from_hira[i]) return to_hira[i];
					return ch;
				}
			public:
				shiritree_word_() { }
				shiritree_word_(std::string str) { setString(str); }
				void setString(std::string str) {
					std::wstring_convert<std::codecvt_utf8<wchar_t>,wchar_t> cvt;
					wstr_ = cvt.from_bytes(str);
					first_letter_ = to50Letters(wstr_[0]);
					if(wstr_[wstr_.size()-1] == L'ー') last_letter_ = to50Letters(wstr_[wstr_.size()-2]);
					else if(wstr_[wstr_.size()-1] == L'ー') last_letter_ = to50Letters(wstr_[wstr_.size()-2]);
					else last_letter_ = to50Letters(wstr_[wstr_.size()-1]);
				}
				inline std::wstring getString() const noexcept { return wstr_; }
				inline bool isUse() const noexcept { return isUse_; }
				inline void setUse(bool flag) { isUse_ = flag; }
				inline wchar_t first() const noexcept { return first_letter_; }
				inline wchar_t last() const noexcept { return last_letter_; }
		};
		private:
			std::vector<std::string> string_words_;
			std::vector<shiritree_word_> shiritree_words_;
			std::vector<std::size_t> continuous_idx_;
			std::vector<std::size_t> max_continuous_idx_;
			std::string spaces_for_print_ = "";
			std::size_t words_count_ = 0;
			std::size_t now_length_ = 0;
			std::size_t max_length_ = 0;
			bool logging_ = true;
			void dfs();
			inline void log_print(std::string str) const;
			inline void log_print_r(std::string str) const;
		public:
			ShiriTree(){}
			ShiriTree(std::string filename){ load(filename); }
			void load(std::string filename);
			void solve();
			inline void view_words() const;
			inline void view_continuous_words() const;
			void output(std::string filename) const;
			void report(std::string filename) const;
			inline std::size_t length() const noexcept { return max_length_; }
			inline void setLogging(bool flag) noexcept { logging_ = flag; }
			inline bool getLogging() const noexcept { return logging_; }
			inline std::size_t wordSize() const noexcept { return words_count_; }
			inline double usageRate() const { return (double)max_length_/(double)words_count_; }
	};
}

void YSL::ShiriTree::load(std::string filename){
	log_print("Loading words by " + filename);
	std::ifstream fin(filename);
	{
		std::string row_data;
		std::getline(fin, row_data);
		words_count_ = count(row_data.begin(), row_data.end(), ',') + 1;
		string_words_.resize(words_count_);
		std::stringstream ss(row_data);
		for(std::size_t i=0; i<words_count_; ++i) std::getline(ss, string_words_[i], ',');
		std::sort(string_words_.begin(), string_words_.end());
		shiritree_words_.resize(words_count_);
		std::size_t max_strlen = 0;
		for(std::size_t i=0; i<words_count_; ++i){
			max_strlen = std::max(max_strlen, string_words_[i].length());
			shiritree_words_[i].setString(string_words_[i]);
		}
		spaces_for_print_.resize(max_strlen*2, ' ');
		continuous_idx_.resize(words_count_);
	}
	return;
}
void YSL::ShiriTree::dfs(){
	log_print_r("seaching lenth : " + std::to_string(now_length_) + "  last word : " + string_words_[continuous_idx_[now_length_-1]]);
	if(now_length_ > max_length_){
		log_print("now lenth is " + std::to_string(now_length_));
		if(logging_) view_continuous_words();
		max_length_ = now_length_;
		max_continuous_idx_ = continuous_idx_;
	}
	if(shiritree_words_[continuous_idx_[now_length_-1]].last() == L'ン') return;
	if(shiritree_words_[continuous_idx_[now_length_-1]].last() == L'ん') return;
	for(std::size_t i=0; i<shiritree_words_.size(); ++i){
		if(!shiritree_words_[i].isUse()){
			if(shiritree_words_[i].first() == shiritree_words_[continuous_idx_[now_length_-1]].last()){
				continuous_idx_[now_length_] = i;
				now_length_++;
				shiritree_words_[i].setUse(true);
				dfs();
				shiritree_words_[i].setUse(false);
				now_length_ --;
			}
		}
	}
}
void YSL::ShiriTree::solve(){
	log_print("Solver is started.");
	now_length_ = 0;
	max_length_ = 0;
	for(std::size_t i=0; i<shiritree_words_.size(); ++i){
		continuous_idx_[0] = i;
		now_length_++;
		shiritree_words_[i].setUse(true);
		dfs();
		shiritree_words_[i].setUse(false);
		now_length_--;
	}
	log_print("Solver is finished.");
}
inline void YSL::ShiriTree::view_words() const {
	for(const auto& e : string_words_) std::cout << e << " ";
	std::cout << std::endl;
	return;
}
inline void YSL::ShiriTree::view_continuous_words() const {
	for(std::size_t i=0; i<max_length_; ++i){
		std::cout << string_words_[max_continuous_idx_[i]] << " ";
	}
	std::cout << std::endl;
	return;
}
void YSL::ShiriTree::output(std::string filename) const {
	log_print("Outputing file by " + filename);
	std::ofstream fout(filename);
	fout << max_length_ << " words connected." << std::endl;
	for(std::size_t i=0; i<max_length_; ++i){
		fout << string_words_[continuous_idx_[i]] << " ";
	}
	fout << std::endl;
	return;
}
void YSL::ShiriTree::report(std::string filename) const {
	log_print("Reporting to " + filename);
	std::ofstream fout(filename);
	fout << "Loaded    Words Length : " << wordSize() << std::endl;
	fout << "Connected Words Length : " << length() << std::endl;
	fout << "Words Usage Rate        : " << usageRate() << std::endl;
	fout << "-- Loaded Words ------------------------------------------" << std::endl;
	for(std::size_t i=0; i<string_words_.size(); ++i){
		fout << string_words_[i] << " ";
	}
	fout << std::endl;
	fout << "-- Connected Words ---------------------------------------" << std::endl;
	for(std::size_t i=0; i<max_length_; ++i){
		fout << string_words_[continuous_idx_[i]] << " ";
	}
	fout << std::endl;
	fout << "----------------------------------------------------------" << std::endl;
	return;
}
inline void YSL::ShiriTree::log_print(std::string str) const {
	if(logging_) std::cout << "[LOG] " << str << spaces_for_print_ << std::endl;
}
inline void YSL::ShiriTree::log_print_r(std::string str) const {
	if(logging_) std::cout << "[LOG] " << str << spaces_for_print_ << "\r";
}
#endif
