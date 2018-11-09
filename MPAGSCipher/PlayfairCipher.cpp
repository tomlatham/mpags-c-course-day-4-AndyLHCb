#include "PlayfairCipher.hpp"

PlayfairCipher::PlayfairCipher( const std::string& key )
{
	//store the original key
	key_ = key;	

	//append the alphabet
	std::copy( alphabet_.begin(),
			alphabet_.end(),
			std::back_inserter(key_) );

	//make sure the key is upper case
	std::transform( key_.begin(),
			key_.end(),
			key_.begin(),
			::toupper);

	//remove non-alpha characters
	auto iter = std::copy_if( key_.begin(),
			key_.end(),
			key_.begin(),
			::isalpha);

	key_.erase(iter, key_.end());

	//change J -> I
	std::transform( key_.begin(),
			key_.end(),
			key_.begin(),
			[](char c){return c == 'J' ? 'I' : c ;} );	

	//remove duplicated letters
	
	std::string soFar{""}; //Logs all letters found so far

	auto dupli = [&](char c){	//Function to find duplicates from soFar
		bool isDupli = std::find(soFar.begin(),
				soFar.end(),
				c) != soFar.end();
		soFar += c;
		return isDupli;	
	};
	
	auto iter2 = std::remove_if( key_.begin(),
			key_.end(),
			dupli		
			);	//Puts duplicates at the back;
	
	key_.erase(iter2,key_.end()); //Erase the duplicates

	//store the coords of each letter
	using mapEntry = std::pair< std::vector<size_t>,char >;	
	
	for(unsigned long i{0}; i<25; ++i){
		std::vector<size_t> pos{i/5,i%5};
		
		mapEntry Addition{ std::make_pair(pos,key_[i]) };
		grid_.insert( Addition );
	}

	std::cout << "KEY:" << key_ << std::endl << std::endl;
	
	for(auto p : grid_)
	{
		std::cout << "Letter:" << p.second
			  << "|Pos:(" << p.first[0] << "," << p.first[1]  << ")" << std::endl;
	}	
		


	//store the playfair cipher key map
	
	
}

std::string PlayfairCipher::applyCipher( const std::string& inputText,
					const CipherMode /*cipherMode*/ ) const
{
	// Change J → I
	
	// If repeated chars in a digraph add an X or Q if XX
	
	// if the size of input is odd, add a trailing Z
	
	// Loop over the input in Digraphs
	
	//   - Find the coords in the grid for each digraph
	
	//   - Apply the rules to these coords to get 'new' coords
	
	//   - Find the letter associated with the new coords
	
	// return the text
	return inputText;
}
