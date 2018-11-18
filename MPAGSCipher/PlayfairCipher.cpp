#include "PlayfairCipher.hpp"

PlayfairCipher::PlayfairCipher( const std::string& key )
{
	setKey(key);
}

void PlayfairCipher::setKey( const std::string& key )
{
	//test if a null cipher is being used
	if( key.empty() ){
		std::cerr << "NO KEY GIVEN, NULL ENCRYPTION PERFORMED" << std::endl;
		nullCipher_ = true;
	}

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

	//Function to find duplicates from soFar
	auto dupli = [&](char c){
		bool isDupli = std::find(soFar.begin(),
				soFar.end(),
				c) != soFar.end();
		if ( !isDupli ) { soFar += c; }
		return isDupli;	
	};
	
	auto iter2 = std::remove_if( key_.begin(),
			key_.end(),
			dupli		
			);	//Puts duplicates at the back;
	
	key_.erase(iter2,key_.end()); //Erase the duplicates

	//store the coords of each letter
	alphaGrid_.clear();
	coordGrid_.clear();
	for(unsigned long i{0}; i<25; ++i){//can't use i<alphabetLength_ because we're throwing away J :(
		PlayfairCoords pos{i/5,i%5};
		alphaGrid_.insert( std::make_pair(key_[i],pos) );
		coordGrid_.insert( std::make_pair(pos,key_[i]) );
	}


	//Printing the key and its coords
	
	std::cout << "KEY:" << key_ << std::endl << std::endl;
	
	for(auto p : alphaGrid_)
	{
		std::cout << "Letter:" << p.first
			  << "|Pos:(" << p.second.first << "," << p.second.second  << ")" << std::endl;
	}	


	std::cout << std::endl;

	//Printing the actual playfair square
	for(size_t j{0}; j<5; j++)
	{
		for(size_t i{0}; i<5; i++)
		{
			PlayfairCoords coords = {j,i};
			std::cout << coordGrid_.find(coords)->second << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;		
}

std::string PlayfairCipher::applyCipher( const std::string& inputText,
					const CipherMode cipherMode ) const
{
	std::string outputText;
	// Change J → I
	std::transform( inputText.begin(),
			inputText.end(),
			std::back_inserter(outputText),
			[](char c){return c == 'J' ? 'I' : c ;} );	

	// If repeated chars in a digraph add an X or Q if XX
	// Processing like this is done in this block (while loop)

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

	//if no key given, no encryption is performed (as per the help message
	if(nullCipher_)
	{
		std::cerr << "RETURNING UNCIPHERED TEXT:" << std::endl;
		return outputText;
	}

	const size_t direction { cipherMode == CipherMode::Encrypt ? 1u : 4u };

	//This block performs the transformation on the letters
	for(size_t i{0}; i<outputText.size(); i+=2)
	{
		PlayfairCoords transformedCoords1;
		PlayfairCoords transformedCoords2;

		auto l1_iter = alphaGrid_.find(outputText[i]);
		auto l2_iter = alphaGrid_.find(outputText[i+1]);
		
		//for testing purposses, these 2 lines output the coords of every digraph if uncommented
		//std::cout<< l1_iter->second.first << "," << l1_iter->second.second << std::endl
		//	 << l2_iter->second.first << "," << l2_iter->second.second << std::endl << std::endl;
		
	
		//are they aligned in x?
		if(l1_iter->second.first == l2_iter->second.first)
		{
			transformedCoords1.first = l1_iter->second.first;
			transformedCoords1.second = (l1_iter->second.second+direction)%5;

			transformedCoords2.first = l2_iter->second.first;
			transformedCoords2.second = (l2_iter->second.second+direction)%5;
		}

		//are they aligned in y?
		else if(l1_iter->second.second == l2_iter->second.second)
		{
			transformedCoords1.first  = (l1_iter->second.first+direction)%5;
			transformedCoords1.second = l1_iter->second.second;

			transformedCoords2.first  = (l2_iter->second.first+direction)%5;
			transformedCoords2.second = l2_iter->second.second;
		}

		//unaligned (rectangle case/corner swap)
		else
		{
			transformedCoords1.first  = l1_iter->second.first;
			transformedCoords1.second = l2_iter->second.second;

			transformedCoords2.first  = l2_iter->second.first;
			transformedCoords2.second = l1_iter->second.second;
		}

		outputText[i]   = coordGrid_.find(transformedCoords1)->second;
		outputText[i+1] = coordGrid_.find(transformedCoords2)->second;
	}

	return outputText;
}
