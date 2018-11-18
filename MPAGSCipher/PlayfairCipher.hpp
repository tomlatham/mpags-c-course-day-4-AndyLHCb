#ifndef MPAGS_PLAYFAIRCIPHER_HPP
#define MPAGS_PLAYFAIRCIPHER_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

#include "CipherMode.hpp"
#include "TransformChar.hpp"

/**
 * \file PlayfairCipher.hpp
 * \brief Contains the declaration of the PlayfairCipher class
 */

/**
 * \class PlayfairCipher
 * \brief Encrypt or decrypt text using the Playfair cipher with a given key
 */

class PlayfairCipher{

	private:
		/// The cipher key, phrase to be used
		std::string key_{"DFAULTKEY"};

		/// The alphabet - used to determine the cipher character given the plain character and the key
		const std::vector<char> alphabet_ = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

		/// The size of the alphabet
		const std::vector<char>::size_type alphabetSize_ = alphabet_.size();

		/// If no key is given, a null cipher is used (no ciphering)
		bool nullCipher_{false};

		/// Type definition for the coordinates in the 5x5 table
		using PlayfairCoords = std::pair<size_t,size_t>;

		/// 5x5 grid to be used for playfair cihper in both directions
		std::map< char,PlayfairCoords > alphaGrid_;
		std::map< PlayfairCoords,char > coordGrid_;
	public:
		/**
		 * create a new PlayfairCipher, converting the given string into the key
		 *
		 * \param key the string to be used as the key
		 */
		explicit PlayfairCipher( const std::string& key );

		/**
		 * Apply the cipher to the provided text
		 *
		 * \param inputText the text to encrypt or decrypt
		 * \param cipherMode whether to encrypt or decrypt the input text
		 * \return the result of applying the cipher to the input text
		 */
		std::string applyCipher( const std::string& inputText,
				const CipherMode cipherMode ) const;

		/**
		 * Change the key being used by the cipher
		 *
		 * \param key the new key to be used
		 */
		void setKey(const std::string& key);
};



#endif
