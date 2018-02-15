/* Based on orf.cpp from NCBI C++ Toolkit
 * License:
 * $Id: orf.cpp 65735 2014-12-23 18:23:27Z astashya $
 * ===========================================================================
 *
 *                            PUBLIC DOMAIN NOTICE
 *               National Center for Biotechnology Information
 *
 *  This software/database is a "United States Government Work" under the
 *  terms of the United States Copyright Act.  It was written as part of
 *  the author's official duties as a United States Government employee and
 *  thus cannot be copyrighted.  This software/database is freely available
 *  to the public for use. The National Library of Medicine and the U.S.
 *  Government have not placed any restriction on its use or reproduction.
 *
 *  Although all reasonable efforts have been taken to ensure the accuracy
 *  and reliability of the software and data, the NLM and the U.S.
 *  Government do not and cannot warrant the performance or results that
 *  may be obtained by using this software or data. The NLM and the U.S.
 *  Government disclaim all warranties, express or implied, including
 *  warranties of performance, merchantability or fitness for any particular
 *  purpose.
 *
 *  Please cite the author in any work or product based on this material.
 *
 * ===========================================================================
 *
 * Authors:  Mike DiCuccio
 *
 * File Description:
 *
 */

#include "Orf.h"
#include "Util.h"
#include "Debug.h"
#include <cstring>
#include <cassert>
#include <cstdlib>
#include "TranslateNucl.h"

//note: N->N, S->S, W->W, U->A, T->A
static const char* iupacReverseComplementTable =
"................................................................"
".TVGH..CD..M.KN...YSAABW.R.......tvgh..cd..m.kn...ysaabw.r......"
"................................................................"
"................................................................";

inline char complement(const char c)
{
    return iupacReverseComplementTable[static_cast<unsigned char>(c)];
}

Orf::Orf() : sequence(NULL), reverseComplement(NULL) {
    TranslateNucl translateNucl(static_cast<TranslateNucl::GenCode>(1)); // standard
    stopCodons = translateNucl.getStopCodons();
}

bool Orf::setSequence(const char* seq, size_t length) {
    cleanup();

    sequenceLength = length;

    if(sequenceLength < 3)
        return false;

    sequence = strdup(seq);
    for(size_t i = 0; i < sequenceLength; ++i) {
        sequence[i] = static_cast<char>(toupper(static_cast<int>(seq[i])));
    }

    reverseComplement = strdup(sequence);
    for(size_t i = 0; i < sequenceLength; ++i) {
        reverseComplement[i] = complement(sequence[sequenceLength - i - 1]);
        if (reverseComplement[i] == '.') {
            return false;
        }
    }

    return true;
}

void Orf::cleanup()  {
    if (sequence) {
        free(sequence);
        sequence = NULL;
    }
    if (reverseComplement) {
        free(reverseComplement);
        reverseComplement = NULL;
    }
}

std::string Orf::view(const SequenceLocation &location) {
    assert(location.to > location.from);
    
    size_t length = location.to - location.from;
    if(location.strand == Orf::STRAND_PLUS) {
        return sequence ? std::string(&sequence[location.from], length) : std::string();
    } else {
        return reverseComplement ? std::string(&reverseComplement[location.from], length) : std::string();
    }
}

void Orf::findAll(std::vector<Orf::SequenceLocation> &result,
                  const size_t minLength,
                  const size_t maxLength,
                  const size_t maxGaps,
                  const unsigned int forwardFrames,
                  const unsigned int reverseFrames,
                  const unsigned int extendMode)
{
    if (forwardFrames != 0) {
        // find ORFs on the forward sequence
        findForward(sequence, sequenceLength, result,
                    minLength, maxLength, maxGaps, forwardFrames, extendMode, STRAND_PLUS);
    }

    if (reverseFrames != 0) {
        // find ORFs on the reverse complement
        findForward(reverseComplement, sequenceLength, result,
                    minLength, maxLength, maxGaps, reverseFrames, extendMode, STRAND_MINUS);
    }
}

inline bool isIncomplete(const char* codon) {
    return codon[0] == 0 || codon[1] == 0 || codon[2] == 0;
}

inline bool isGapOrN(const char *codon) {
    return codon[0] == 'N' || complement(codon[0]) == '.'
        || codon[1] == 'N' || complement(codon[1]) == '.'
        || codon[2] == 'N' || complement(codon[2]) == '.';
}

inline bool isStart(const char* codon) {
    return (codon[0] == 'A' && codon[1] == 'U' && codon[2] == 'G')
        || (codon[0] == 'A' && codon[1] == 'T' && codon[2] == 'G');
}

bool Orf::isStop(const char* codon) {
    
    char nuc0 = codon[0];
    char nuc1 = codon[1];
    char nuc2 = codon[2];

    // replace U with T before checking stop codons:
    if (nuc0 == 'U') {
        nuc0 = 'T';
    }
    if (nuc1 == 'U') {
        nuc1 = 'T';
    }
    if (nuc2 == 'U') {
        nuc2 = 'T';
    }

    for (size_t stopCodInd = 0; stopCodInd < stopCodons.size(); ++stopCodInd) {
        if (nuc0 == stopCodons[stopCodInd][0] && nuc1 == stopCodons[stopCodInd][1] && nuc2 == stopCodons[stopCodInd][2]) {
            return true;
        }
    }

    return false;
    /*
    return (codon[0] == 'U' && codon[1] == 'A' && codon[2] == 'G')
        || (codon[0] == 'U' && codon[1] == 'A' && codon[2] == 'A')
        || (codon[0] == 'U' && codon[1] == 'G' && codon[2] == 'A')
        || (codon[0] == 'T' && codon[1] == 'A' && codon[2] == 'G')
        || (codon[0] == 'T' && codon[1] == 'A' && codon[2] == 'A')
        || (codon[0] == 'T' && codon[1] == 'G' && codon[2] == 'A');
    */
     
    // Eli change:
    //TranslateNucl translateNucl(static_cast<TranslateNucl::GenCode>(1)); // standard
    //char aa[2]; // exactly one aa
    //translateNucl.translate(aa, codon, 1);
    //if (aa[0] == '*')
    //{
    //    return true;
    //}
    //else
    //{
    //    return false;
    //}
}


void Orf::findForward(const char *sequence, const size_t sequenceLength, std::vector<SequenceLocation> &result,
                      const size_t minLength, const size_t maxLength, const size_t maxGaps, const unsigned int frames,
                      const unsigned int extendMode, const Strand strand) {
    // An open reading frame can beginning in any of the three codon start position
    // Frame 0:  AGA ATT GCC TGA ATA AAA GGA TTA CCT TGA TAG GGT AAA
    // Frame 1: A GAA TTG CCT GAA TAA AAG GAT TAC CTT GAT AGG GTA AA
    // Frame 2: AG AAT TGC CTG AAT AAA AGG ATT ACC TTG ATA GGG TAA A
    const int FRAMES = 3;
    const int frameLookup[FRAMES] = {FRAME_1, FRAME_2, FRAME_3};
    const size_t frameOffset[FRAMES] = {0, 1 , 2};

    // We want to walk over the memory only once so we calculate which codon we are in
    // and save the values of our state machine in these arrays

    // we also initialize our state machine with being inside an orf
    // this is to handle edge case 1 where we find an end codon but no start codon
    // in this case we just add an orf from the start to the found end codon
    bool isInsideOrf[FRAMES]     = {true,  true,  true };
    bool hasStartCodon[FRAMES]   = {false, false, false};

    size_t countGaps[FRAMES]   = {0, 0, 0};
    size_t countLength[FRAMES] = {0, 0, 0};

    // Offset the start position by reading frame
    size_t from[FRAMES] = {frameOffset[0], frameOffset[1], frameOffset[2]};

    for (size_t i = 0;  i < sequenceLength - (FRAMES - 1);  i += FRAMES) {
        for(size_t position = i; position < i + FRAMES; position++) {
            const char* codon = sequence + position;
            size_t frame = position % FRAMES;

            // skip frames outside of out the frame mask
            if(!(frames & frameLookup[frame])) {
                continue;
            }

            bool thisIncomplete = isIncomplete(codon);
            bool isLast = !thisIncomplete && isIncomplete(codon + FRAMES);

            // if we have the start extend mode the returned orf should return the longest
            // possible orf with possibly multiple start codons
            
            bool eli_mode = true;

            bool shouldStart;
            if((extendMode & EXTEND_START)) {
                shouldStart = isInsideOrf[frame] == false && isStart(codon);
            } else if (eli_mode) {
                shouldStart = isInsideOrf[frame] == false;
            } 
            else {
                shouldStart = isStart(codon);
            }
            

            // do not start a new orf on the last codon
            if(shouldStart && isLast == false) {
                isInsideOrf[frame] = true;
                hasStartCodon[frame] = true;
                from[frame] = position;

                countGaps[frame] = 0;
                countLength[frame] = 0;
            }

            if(isInsideOrf[frame]) {
                countLength[frame]++;

                if(isGapOrN(codon)) {
                    countGaps[frame]++;
                }
            }

            bool stop = isStop(codon);
            if(isInsideOrf[frame] && (stop || isLast)) {
                // possibly bail early if we have an orf shorter than minLength
                // so we can find another longer one
                 if ((extendMode & EXTEND_END) && stop && countLength[frame] <= minLength) {
                    continue;
                 }

                isInsideOrf[frame] = false;

                // we include the stop codon here
                size_t to = position + (isLast ? 3 : 0);

                // this could happen if the first codon is a stop codon
                if(to == from[frame])
                    continue;

                assert(to > from[frame]);

                // ignore orfs with too many gaps or unknown codons
                // also ignore orfs shorter than the min size and longer than max
                if ((countGaps[frame] > maxGaps)
                || (countLength[frame] > maxLength)
                || (countLength[frame] <= minLength)) {
                    continue;
                }

                result.emplace_back(SequenceLocation(from[frame], to,
                                                     !hasStartCodon[frame], !stop, strand));
            }
        }
    }
}

Orf::SequenceLocation Orf::parseOrfHeader(char *data) {
    char * entry[255];
    size_t columns = Util::getWordsOfLine(data, entry, 255);
    size_t col;
    bool found = false;
    for(col = 0; col < columns; col++){
        if(entry[col][0] == '[' && entry[col][1] == 'O' && entry[col][2] == 'r' && entry[col][3] == 'f' && entry[col][4] == ':'){
            found=true;
            break;
        }
    }
    if(found==false ){
        Debug(Debug::ERROR) << "Could not find Orf information in header.\n";
        EXIT(EXIT_FAILURE);
    }
    Orf::SequenceLocation loc;
    int strand;
    int hasIncompleteStart, hasIncompleteEnd;
    int retCode = sscanf(entry[col], "[Orf: %u, %zu, %zu, %d, %d, %d]", &loc.id, &loc.from, &loc.to, &strand, &hasIncompleteStart, &hasIncompleteEnd);
    loc.hasIncompleteStart = hasIncompleteStart;
    loc.hasIncompleteEnd = hasIncompleteEnd;
    if(retCode < 5){
        Debug(Debug::ERROR) << "Could not parse Orf " << entry[col] << ".\n";
        EXIT(EXIT_FAILURE);
    }
    loc.strand =  static_cast<Orf::Strand>(strand);
    return loc;
}
