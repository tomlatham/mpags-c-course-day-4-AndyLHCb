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
	using mapEntry = std::pair< char,std::vector<size_t> >;	
	using mapEntryCo = std::pair < std::vector<size_t>,char >;

	for(unsigned long i{0}; i<25; ++i){//can't use i<alphabetLength_ because we're throwing away J :(
		std::vector<size_t> pos{i/5,i%5};
		
		mapEntry Addition{ std::make_pair(key_[i],pos) };
		mapEntryCo Addition2{ std::make_pair(pos,key_[i]) };
		alphaGrid_.insert( Addition );
		coordGrid_.insert( Addition2);
	}


	//You can print the key by uncommenting this:
	
	std::cout << "KEY:" << key_ << std::endl << std::endl;
	
	for(auto p : alphaGrid_)
	{
		std::cout << "Letter:" << p.first
			  << "|Pos:(" << p.second[0] << "," << p.second[1]  << ")" << std::endl;
	}	

	/* uncomment to print out coordGrid_
	 *
	for(auto p : coordGrid_)
	{
		std::cout << "Pos:(" << p.first[0] << "," << p.first[1] << ")"
			  << "|Letter:" << p.second << std::endl;
	}*/
	std::cout << std::endl;		


	//store the playfair cipher key map
	//this has already been done	
	
}

std::string PlayfairCipher::applyCipher( const std::string& inputText,
					const CipherMode /*cipherMode*/ ) const
{
	std::string outputText;
	// Change J → I
	std::transform( inputText.begin(),
			inputText.end(),
			std::back_inserter(outputText),
			[](char c){return c == 'J' ? 'I' : c ;} );	

	// If repeated chars in a digraph add an X or Q if XX
	
	std::string tempString;
	size_t counter{0};

	while( counter < outputText.length() )
	{
		tempString += outputText[counter];
		//dealing with odd numbers of letters
		if(counter == outputText.length()-1)
		{
			tempString += (outputText[counter] == 'Z' ? 'X' : 'Z');
			break;
		}
		if(outputText[counter+1] == outputText[counter])
		{
			tempString += (outputText[counter] == 'X' ? 'Q' : 'X');
			++counter;
		}
		else
		{
			tempString += outputText[counter+1];
			counter += 2;
		}
	}
	outputText = tempString;

	for(size_t i{0}; i<outputText.size(); i+=2)
	{
		auto l1_iter = alphaGrid_.find(outputText[i]);
		auto l2_iter = alphaGrid_.find(outputText[i+1]);
		std::cout<< l1_iter->second[0] << "," << l1_iter->second[1] << std::endl
			 << l2_iter->second[0] << "," << l2_iter->second[1] << std::endl << std::endl;
	}

	return outputText;
}



