/**
 * @file tokenizer.h
 */

#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include <memory>
#include <string>
#include <unordered_map>
#include "index/structs.h"
#include "index/document.h"
#include "util/invertible_map.h"

namespace meta {
namespace tokenizers {

/**
 * An class that provides a framework to produce tokens. All tokenizers inherit
 * from this class and (possibly) implement tokenize(). This abstract base class
 * does the bookkeeping of mapping term strings to term IDs.
 */
class tokenizer
{
    public:

        /**
         * Constructor. Simply initializes some member variables that
         *  keep track of the TermID mapping.
         */
        tokenizer();

        /**
         * A default virtual destructor.
         */
        virtual ~tokenizer() = default;

        /**
         * Tokenizes a file into a Document.
         * @param document - the Document to store the tokenized information in
         * @param docFreq - optional parameter to store IDF values in
         */
        virtual void tokenize(index::Document & document,
                const std::shared_ptr<std::unordered_map<index::TermID, unsigned int>> & docFreq = nullptr) = 0;

        /**
         * Maps terms to TermIDs.
         * @param term - the term to check
         * @return the TermID assigned to this term
         */
        virtual index::TermID mapping(const std::string & term);

        /**
         * Calls the TermID InvertibleMap's saveMap function.
         * @param filename - the filename to save the mapping as
         */
        virtual void save_term_id_mapping(const std::string & filename) const;

        /**
         * Sets the token to termid mapping for this tokenizer.
         * This is useful when reading an inverted index from disk
         *  with an existing mapping.
         * @param mapping - a reference to the desired mapping
         */
        virtual void set_term_id_mapping(const util::InvertibleMap<index::TermID, std::string> & mapping);

        /**
         * @return a reference to the structure used to store the termID <->
         * term string mapping
         */
        virtual const util::InvertibleMap<index::TermID, std::string> & term_id_mapping() const;

        /**
         * TODO there are probably other functions that MultiTokenizer messes up
         * Looks up the actual label that is represented by a TermID.
         * @param termID
         * @return the label
         */
        virtual std::string label(index::TermID termID) const;

        /**
         * Prints the data associated with this tokenizer, consisting of a TermID and its string
         * value.
         */
        virtual void print_data() const;

        /**
         * @return the number of terms seen so far by this tokenizer
         */
        virtual size_t num_terms() const;

        /**
         * Sets the current termID for this tokenizer. This is useful when running multiple
         * tokenizers on a single documents.
         */
        virtual void set_max_term_id(size_t start);

        /**
         * @return the max TermID associated with this Tokenizer. This is NOT
         * the total number of unique terms seen by this Tokenizer.
         */
        virtual index::TermID max_term_id() const;

    protected:

        /**
         * Keeps track of the internal mapping of TermIDs to strings parsed
         * from the file. This is protected mainly so MultiTokenizer can update
         * its _termMap correctly.
         */
        util::InvertibleMap<index::TermID, std::string> _term_map;
 
    private:

        /**
         * Internal counter for the number of unique terms seen (used as keys
         * in the InvertibleMap).
         */
        index::TermID _current_term_id;
};

}
}

#endif