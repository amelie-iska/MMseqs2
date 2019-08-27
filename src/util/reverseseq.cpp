#include "Parameters.h"
#include "DBReader.h"
#include "DBWriter.h"
#include "Debug.h"
#include "Util.h"
#include "FileUtil.h"

#ifdef OPENMP
#include <omp.h>
#endif

int reverseseq(int argn, const char **argv, const Command& command) {
    Parameters& par = Parameters::getInstance();
    par.parseParameters(argn, argv, command, true, true, 0);

    DBReader<unsigned int> seqReader(par.db1.c_str(), par.db1Index.c_str(), par.threads, DBReader<unsigned int>::USE_INDEX|DBReader<unsigned int>::USE_DATA);
    seqReader.open(DBReader<unsigned int>::LINEAR_ACCCESS);

    DBWriter revSeqWriter(par.db2.c_str(), par.db2Index.c_str(), par.threads, par.compressed, seqReader.getDbtype());
    revSeqWriter.open();
    Debug::Progress progress(seqReader.getSize());

#pragma omp parallel
    {
        unsigned int thread_idx = 0;
#ifdef OPENMP
        thread_idx = static_cast<unsigned int>(omp_get_thread_num());
#endif
        std::string revStr;
        revStr.reserve(32000);

#pragma omp for schedule(dynamic, 100)
        for (size_t id = 0; id < seqReader.getSize(); id++) {
            progress.updateProgress();
            unsigned int seqKey = seqReader.getDbKey(id);
            char *seq = seqReader.getData(id, thread_idx);
            size_t lenSeq = seqReader.getSeqLen(id);
            for (size_t i = 0; i < lenSeq; ++i) {
                revStr.push_back(seq[lenSeq - i - 1]);
            }
            revStr.push_back('\n');
            revSeqWriter.writeData(revStr.c_str(), revStr.size(), seqKey, thread_idx, true);
            revStr.clear();
        }
    }
    revSeqWriter.close();
    seqReader.close();
    DBReader<unsigned int>::softlinkDb(par.db1, par.db2, DBFiles::SEQUENCE_ANCILLARY);

    return EXIT_SUCCESS;
}
