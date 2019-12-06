#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <random>
#include <limits>
#include <cctype>

bool VectorContainsWord(const std::vector<std::string>& vec, const std::string& word)
{
	for (const auto& w : vec)
	{
		if (w == word)
		{
			return true;
		}
	}

	return false;
}

std::vector<int> FillBuckets(const std::string& word)
{
	std::vector<int> buckets(26, 0);

	for (char c : word)
	{
		buckets[c - 'a']++;
	}

	return buckets;
}

int ScoreMatch(const std::string& word1, const std::string& word2)
{
	const std::vector<int> buckets1 = FillBuckets(word1);
	const std::vector<int> buckets2 = FillBuckets(word2);

	int score = 0;

	for (int i = 0; i < 26; ++i)
	{
		score += std::min(buckets1[i], buckets2[i]);
	}
	 
	for (int i = 0; i < 5; ++i)
	{
		if (word1[i] == word2[i])
		{
			score++;
		}
	}

	return score;
}

int main()
{
	std::vector<std::string> five_words;
	std::vector<std::string> common_words;

	{
		std::ifstream common_word_file("20k.txt");
		
		for (std::string line; std::getline(common_word_file, line); )
		{
			if (line.empty() || line.size() != 5)
			{
				continue;
			}
			common_words.emplace_back(line);
		}
	}

	{
		std::ifstream five_word_file("sgb-words.txt");
		
		for (std::string line; std::getline(five_word_file, line); )
		{
			if (line.empty())
			{
				continue;
			}
			for (const std::string& com : common_words)
			{
				if (line == com)
				{
					five_words.emplace_back(line);
					break;
				}
			}
		}
	}

	std::mt19937 rng( std::random_device{}() );
	std::uniform_int_distribution<int> dist(0, five_words.size() - 1);
	const std::string target = five_words[dist(rng)];

	while (true)
	{
		std::cout << "Guess a-five-letter-word ('quit' to quit, duh): ";
		std::string guess;
		std::getline(std::cin, guess);
		
		for (char& c : guess)
		{
			c = std::tolower(c);
		}

		if (guess == "quit")
		{
			std::cout << "Quitting..." << std::endl;
			break;
		}

		if (guess.size() != 5)
		{
			std::cout << "I said 5 letters mf..\n";
			continue;
		}

		if (!VectorContainsWord(five_words, guess))
		{
			std::cout << guess << "? is not evey a word!\n";
			continue;
		}

		const int score = ScoreMatch(guess, target);

		if (score == 10)
		{
			std::cout << "You did it!!, warraou warraou!" << std::endl;
		}
		else
		{
			std::cout << "Nah, it ain't getting it, I'll give you " << score << " points, tho." << std::endl;
		}
	}

	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.get();

	return 0;
}