#include "Debug.h"
#include "Command.h"
#include "CommandDeclarations.h"
#include "Util.h"
#include "Parameters.h"

const char* binary_name = "mmseqs";
const char* tool_name = "MMseqs2";
const char* tool_introduction = "MMseqs2 (Many against Many sequence searching) is an open-source software suite for very fast, \nparallelized protein sequence searches and clustering of huge protein sequence data sets.\n\nPlease cite: M. Steinegger and J. Soding. MMseqs2 enables sensitive protein sequence searching for the analysis of massive data sets. Nature Biotechnology, doi:10.1038/nbt.3988 (2017).";
const char* main_author = "Martin Steinegger (martin.steinegger@mpibpc.mpg.de)";
const char* show_extended_help = "1";
const char* show_bash_info = "1";

Parameters& par = Parameters::getInstance();

std::vector<struct Command> commands = {
// Main tools  (for non-experts)
        {"createdb",             createdb,             &par.createdb,             COMMAND_MAIN,
                "Convert protein sequence set in a FASTA file to MMseqs sequence DB format",
                "converts a protein sequence flat/gzipped FASTA or FASTQ file to the MMseqs sequence DB format. This format is needed as input to mmseqs search, cluster and many other tools.",
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:fastaFile1[.gz]> ... <i:fastaFileN[.gz]> <o:sequenceDB>",
                CITATION_MMSEQS2},
        {"easy-search",          easysearch,           &par.easysearchworkflow,       COMMAND_EASY,
                "Search with a query fasta against target fasta (or database) and return a BLAST-compatible result in a single step",
                "Searches with a sequence FASTA file through the target sequence FASTA file or DB by in a single step. This combines createdb, search, summarizeresults, convert and convertalis modules into a single workflow.",
                "Milot Mirdita <milot@mirdita.de> & Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:queryFastaFile[.gz]>  <i:targetFastaFile[.gz]>|<i:targetDB> <o:alignmentFile> <tmpDir>",
                CITATION_MMSEQS2},
        {"easy-linclust",          easylinclust,           &par.linclustworkflow,       COMMAND_EASY,
                "Compute clustering of a fasta database in linear time. The workflow outputs the representative sequences, a cluster tsv and a fasta-like format containing all sequences.",
                "Clusters sequences by similarity in linear time. It groups similar sequences together based on user-specified criteria (max. E-value, seq. id., min. coverage,...).",
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:queryFastaFile[.gz]> <o:clusterPrefix> <tmpDir>",
                CITATION_MMSEQS2},
        {"easy-cluster",          easycluster,           &par.clusteringWorkflow,       COMMAND_EASY,
                "Compute clustering of a fasta database. The workflow outputs the representative sequences, a cluster tsv and a fasta-like format containing all sequences.",
                "Clusters sequences by similarity  It compares all sequences in the sequence DB with each other using mmseqs search, filters alignments according to user-specified criteria (max. E-value, min. coverage,...),   and runs mmseqs clust to group similar sequences together into clusters.",
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:queryFastaFile[.gz]> <o:clusterPrefix> <tmpDir>",
                CITATION_MMSEQS2},
        {"search",               search,               &par.searchworkflow,       COMMAND_MAIN,
                "Search with query sequence or profile DB (iteratively) through target sequence DB",
                "Searches with the sequences or profiles query DB through the target sequence DB by running the prefilter tool and the align tool for Smith-Waterman alignment. For each query a results file with sequence matches is written as entry into a database of search results (alignmentDB).\nIn iterative profile search mode, the detected sequences satisfying user-specified criteria are aligned to the query MSA, and the resulting query profile is used for the next search iteration. Iterative profile searches are usually much more sensitive than (and at least as sensitive as) searches with single query sequences.",
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:queryDB> <i:targetDB> <o:alignmentDB> <tmpDir>",
                CITATION_MMSEQS2},
        {"map",                   map,                  &par.mapworkflow,         COMMAND_MAIN,
                "Fast ungapped mapping of query sequences to target sequences.",
                "Finds very similar sequence matches in a sequence database. First calls the prefilter module (with a low sensitivity setting) to detect high scoring diagonals and then computes an ungapped alignment with the rescorediagonal module. In contrast to the normal search, for maximum speed no gapped alignment is computed, query sequences are not masked for low complexity regions and no compositional bias correction is applied.",
                "Milot Mirdita <milot@mirdita.de> & Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:queryDB> <i:targetDB> <o:alignmentDB> <tmpDir>",
                CITATION_MMSEQS2},
        {"cluster",              clusteringworkflow,    &par.clusteringWorkflow,  COMMAND_MAIN,
                "Compute clustering of a sequence DB (quadratic time)",
                "Clusters sequences by similarity. It compares all sequences in the sequence DB with each other using mmseqs search, filters alignments according to user-specified criteria (max. E-value, min. coverage,...),   and runs mmseqs clust to group similar sequences together into clusters.",
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de> & Lars von den Driesch",
                "<i:sequenceDB> <o:clusterDB> <tmpDir>",
                CITATION_LINCLUST|CITATION_MMSEQS1|CITATION_MMSEQS2},
        {"linclust",          linclust,          &par.linclustworkflow,            COMMAND_MAIN,
                "Cluster sequences of >30% sequence identity *in linear time*",
                "Detects redundant sequences based on reduced alphabet and k-mer sorting.",
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de> ",
                "<i:sequenceDB> <o:clusterDB> <tmpDir>",
                CITATION_MMSEQS2|CITATION_LINCLUST},
        {"indexdb",          indexdb,          &par.indexdb,          COMMAND_HIDDEN,
                "Precompute index table of sequence DB for faster searches",
                "Precomputes an index table for the sequence DB. Handing over the precomputed index table as input to mmseqs search or mmseqs prefilter eliminates the computational overhead of building the index table on the fly.",
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:sequenceDB> <o:sequenceIndexDB>",
                CITATION_MMSEQS2},
        {"createindex",          createindex,          &par.createindex,          COMMAND_MAIN,
                "Precompute index table of sequence DB for faster searches",
                "Precomputes an index table for the sequence DB. Handing over the precomputed index table as input to mmseqs search or mmseqs prefilter eliminates the computational overhead of building the index table on the fly.",
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:sequenceDB> <tmpDir>",
                CITATION_MMSEQS2},
        {"enrich",                enrich,              &par.enrichworkflow,       COMMAND_MAIN,
                "Enrich a query set by searching iteratively through a profile sequence set.",
                "",
                "Milot Mirdita <milot@mirdita.de>",
                "<i:queryDB> <i:targetDB> <o:alignmentDB> <tmpDir>",
                CITATION_MMSEQS2},
// Utility tools for format conversions
        {"createtsv",            createtsv,            &par.createtsv,        COMMAND_FORMAT_CONVERSION,
                "Create tab-separated flat file from prefilter DB, alignment DB, cluster DB, or taxa DB",
                "Create tab-separated flat file from prefilter DB, alignment DB, cluster DB, or taxa DB. The target database is optional. This is useful for taxa DB, since it does not have a target key.",
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:queryDB> [<i:targetDB>] <i:resultDB> <o:tsvFile>",
                CITATION_MMSEQS2},
        {"convertalis",          convertalignments,    &par.convertalignments,    COMMAND_FORMAT_CONVERSION,
                "Convert alignment DB to BLAST-tab format, SAM flat file, or to raw pairwise alignments",
                NULL,
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:queryDb> <i:targetDb> <i:alignmentDB> <o:alignmentFile>",
                CITATION_MMSEQS2},
        {"convertprofiledb",     convertprofiledb,     &par.convertprofiledb,     COMMAND_FORMAT_CONVERSION,
                "Convert ffindex DB of HMM/HMMER3/PSSM files to MMseqs profile DB",
                NULL,
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:ffindexProfileDB> <o:profileDB>",
                CITATION_MMSEQS2},
        {"convert2fasta",        convert2fasta,        &par.convert2fasta,        COMMAND_FORMAT_CONVERSION,
                "Convert sequence DB to FASTA format",
                NULL,
                "Milot Mirdita <milot@mirdita.de>",
                "<i:sequenceDB> <o:fastaFile>",
                CITATION_MMSEQS2},
        {"result2flat",          result2flat,          &par.result2flat,          COMMAND_FORMAT_CONVERSION,
                "Create a FASTA-like flat file from prefilter DB, alignment DB, or cluster DB",
                NULL,
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:queryDB> <i:targetDB> <i:resultDB> <o:fastaDB>",
                CITATION_MMSEQS2},
// Taxonomy
        {"taxonomy",             taxonomy,             &par.taxonomy,             COMMAND_TAXONOMY,
                "Compute taxonomy and lowest common ancestor for each sequence.",
                NULL,
                "Milot Mirdita <milot@mirdita.de>",
                "<i:queryDB> <i:targetDB> <i:targetTaxonMapping> <i:NcbiTaxdmpDir> <o:taxaDB> <tmpDir>",
                CITATION_MMSEQS2
        },
        {"search-2m",             search2m,             &par.taxonomy,             COMMAND_TAXONOMY,
                "Compute taxonomy and lowest common ancestor for each sequence.",
                NULL,
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de> && Milot Mirdita <milot@mirdita.de>",
                "<i:queryDB> <i:targetDB> <i:targetTaxonMapping> <i:NcbiTaxdmpDir> <o:taxaDB> <tmpDir>",
                CITATION_MMSEQS2
        },
        {"addtaxonomy",             addtaxonomy,             &par.onlythreads,             COMMAND_TAXONOMY,
                "Add taxonomy information to result database.",
                NULL,
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                " <i:targetDB> <i:resultDB> <o:resultDB>",
                CITATION_MMSEQS2
        },
        {"lca",                  lca,                  &par.lca,                  COMMAND_TAXONOMY,
                "Compute the lowest common ancestor from a set of taxa.",
                NULL,
                "Milot Mirdita <milot@mirdita.de>",
                "<i:targetDB> <i:resultDB> <o:taxaDB>",
                CITATION_MMSEQS2},
        {"filtertaxdb",                  filtertaxdb,                  &par.filtertaxdb,                  COMMAND_TAXONOMY,
                "Filter taxonomy database.",
                NULL,
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:targetDB> <i:taxaDB> <o:taxaDB>",
                CITATION_MMSEQS2},
// multi hit search
        {"multihitdb",           multihitdb,           &par.multihitdb,           COMMAND_MULTIHIT,
                "Create sequence database and associated metadata for multi hit searches",
                NULL,
                "Clovis Norroy & Milot Mirdita <milot@mirdita.de>",
                "<i:fastaFile1[.gz]> ... <i:fastaFileN[.gz]> <o:setDB> <tmpDir>",
                CITATION_MMSEQS2},
        {"multihitsearch",       multihitsearch,       &par.multihitsearch,       COMMAND_MULTIHIT,
                "Search with a grouped set of sequences against another grouped set",
                NULL,
                "Clovis Norroy & Milot Mirdita <milot@mirdita.de>",
                "<i:querySetDB> <i:targetSetDB> <o:resultDB> <tmpDir>",
                CITATION_MMSEQS2},
        {"besthitperset",        besthitperset,        &par.besthitbyset,         COMMAND_MULTIHIT,
                "For each set of sequences compute the best element and updates the p-value",
                NULL,
                "Clovis Norroy & Milot Mirdita <milot@mirdita.de>",
                " <i:targetSetDB> <i:resultDB> <o:resultDB>",
                CITATION_MMSEQS2},
        {"combinepvalperset",    combinepvalperset,    &par.combinepvalbyset,     COMMAND_MULTIHIT,
                "For each set compute the combined p-value",
                NULL,
                "Clovis Norroy & Milot Mirdita <milot@mirdita.de>",
                "<i:querySetDB> <i:targetSetDB> <i:resultDB> <o:pvalDB>",
                CITATION_MMSEQS2},
        {"summerizeresultsbyset",summerizeresultsbyset,&par.summerizeresultsbyset,COMMAND_MULTIHIT,
                "For each set compute summary statistics, such as spread-pvalue etc.",
                NULL,
                "Clovis Norroy & Milot Mirdita <milot@mirdita.de>",
                "<i:querySetDB> <i:targetSetDB> <i:resultDB> <o:setSummaryDB> <tmpDir>",
                CITATION_MMSEQS2},
        {"resultsbyset",         resultsbyset,         &par.summerizeresultsbyset,COMMAND_MULTIHIT,
                "For each set compute the combined p-value",
                NULL,
                "Clovis Norroy & Milot Mirdita <milot@mirdita.de>",
                "<i:querySetDB> <i:targetSetDB> <i:resultDB> <o:setSummaryDB>",
                CITATION_MMSEQS2},
        {"mergeresultsbyset",    mergeresultsbyset,    &par.onlythreads,          COMMAND_MULTIHIT,
                "Merge results from multiple orfs back to their respective contig",
                NULL,
                "Clovis Norroy & Milot Mirdita <milot@mirdita.de>",
                "<i:setDB> <i:resultDB> <o:resultDB>",
                CITATION_MMSEQS2},
// Utility tools for clustering
        {"clusterupdate",        clusterupdate,        &par.clusterUpdate,        COMMAND_MAIN,
                "Update clustering of old sequence DB to clustering of new sequence DB",
                NULL,
                "Clovis Galiez & Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:oldSequenceDB> <i:newSequenceDB> <i:oldClustResultDB> <o:newMappedSequenceDB> <o:newClustResultDB> <tmpDir>",
                CITATION_MMSEQS2|CITATION_MMSEQS1},
        {"createseqfiledb",      createseqfiledb,      &par.createseqfiledb,      COMMAND_FORMAT_CONVERSION,
                "Create DB of unaligned FASTA files (1 per cluster) from sequence DB and cluster DB",
                NULL,
                "Milot Mirdita <milot@mirdita.de>",
                "<i:sequenceDB> <i:clusterDB> <o:fastaDB>",
                CITATION_MMSEQS2},
        {"mergeclusters",        mergeclusters,        &par.onlythreads,          COMMAND_CLUSTER,
                "Merge multiple cluster DBs into single cluster DB",
                NULL,
                "Maria Hauser & Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:sequenceDB> <o:clusterDB> <i:clusterDB1> ... <i:clusterDBn>",
                CITATION_MMSEQS2},
// Expert tools (for advanced users)
        {"prefilter",            prefilter,            &par.prefilter,            COMMAND_EXPERT,
                "Search with query sequence / profile DB through target DB (k-mer matching + ungapped alignment)",
                "Searches with the sequences or profiles in query DB through the target sequence DB in two consecutive stages: a very fast k-mer matching stage (double matches on same diagonal) and a subsequent ungapped alignment stage. For each query a results file with sequence matches is written as entry into the prefilter DB.",
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de> & Maria Hauser",
                "<i:queryDB> <i:targetDB> <o:prefilterDB>",
                CITATION_MMSEQS2},

        {"ungappedprefilter",            ungappedprefilter,            &par.ungappedprefilter,            COMMAND_EXPERT,
                "Search with query sequence / profile DB through target DB and compute optimal ungapped alignment score",
                "Searches with the sequences or profiles in query DB through the target sequence DB. We compute ungapped alignment score for each diagonal. For each query a results file with sequence matches is written as entry into the prefilter DB.",
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:queryDB> <i:targetDB> <o:prefilterDB>",
                CITATION_MMSEQS2},
        {"align",                align,                &par.align,                COMMAND_EXPERT,
                "Compute Smith-Waterman alignments for previous results (e.g. prefilter DB, cluster DB)",
                "Calculates Smith-Waterman alignment scores between all sequences in the query database and the sequences of the target database which passed the prefiltering.",
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de> & Maria Hauser",
                "<i:queryDB> <i:targetDB> <i:resultDB> <o:alignmentDB>",
                CITATION_MMSEQS2},

        {"alignall",             alignall,                &par.align,                COMMAND_EXPERT,
                "Compute all against all Smith-Waterman alignments for a results (e.g. prefilter DB, cluster DB)",
                "Calculates an all against all Smith-Waterman alignment scores between all sequences in a result. It reports all hits which passed the alignment criteria.",
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:queryDB> <i:targetDB> <i:resultDB> <o:alignmentDB>",
                CITATION_MMSEQS2},
        {"clust",                clust,                &par.clust,                COMMAND_EXPERT,
                "Cluster sequence DB from alignment DB (e.g. created by searching DB against itself)",
                "Computes a clustering of a sequence DB based on the alignment DB containing for each query sequence or profile the Smith Waterman alignments generated by mmseqs align. (When given a prefilter DB as input the tool will use the ungapped alignment scores scores for the clustering.) The tool reads the search results DB,  constructs a similarity graph based on the matched sequences in alignment DB, and applies one of several clustering algorithms. The first, representative sequence of each cluster is connected by an edge to each cluster member. Its names are used as ID in the resulting cluster DB, the entries contain the names of all member sequences.",
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de> & Lars von den Driesch & Maria Hauser",
                "<i:sequenceDB> <i:alignmentDB> <o:clusterDB>",
                CITATION_MMSEQS2|CITATION_MMSEQS1},
        {"kmermatcher",          kmermatcher,          &par.kmermatcher,            COMMAND_EXPERT,
                "Finds exact $k$-mers matches between sequences",
                NULL,
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de> ",
                "<i:sequenceDB> <o:prefDB>",
                CITATION_MMSEQS2},
        {"clusthash",            clusthash,            &par.clusthash,            COMMAND_EXPERT,
                "Cluster sequences of same length and >90% sequence identity *in linear time*",
                "Detects redundant sequences based on reduced alphabet hashing and hamming distance.",
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de> ",
                "<i:sequenceDB> <o:alignmentDB>",
                CITATION_MMSEQS2},
// Utility tools to manipulate DBs
        {"apply",                apply,               &par.onlythreads,          COMMAND_DB,
                "Passes each input database entry to stdin of the specified program, executes it and writes the its stdout to the output database.",
                NULL,
                "Milot Mirdita <milot@mirdita.de>",
                "<i:DB> <o:DB> -- program [args...]",
                CITATION_MMSEQS2},
        {"extractorfs",          extractorfs,          &par.extractorfs,          COMMAND_DB,
                "Extract open reading frames from all six frames from nucleotide sequence DB",
                NULL,
                "Milot Mirdita <milot@mirdita.de>",
                "<i:sequenceDB> <o:sequenceDB>",
                CITATION_MMSEQS2},
        {"orftocontig",             orftocontig,            &par.orftocontig,    COMMAND_DB,
                "Obtain location information of extractd orfs with respect to their contigs in alignment format",
                "Parses extracted orfs headers to computate their coordinates on the contig and writes the results in alignment format",
                "Eli Levy Karin <eli.levy.karin@gmail.com> ",
                "<i:contigsSequenceDB> <i:extractedOrfsHeadersDB> <o:orfsAlignedToContigDB>",
                CITATION_MMSEQS2},
        {"translatenucs",        translatenucs,        &par.translatenucs,        COMMAND_DB,
                "Translate nucleotide sequence DB into protein sequence DB",
                NULL,
                "Milot Mirdita <milot@mirdita.de>",
                "<i:sequenceDB> <o:sequenceDB>",
                CITATION_MMSEQS2},
        {"translateaa",        translateaa,        &par.onlythreads,        COMMAND_DB,
                "Translate protein sequence into nucleotide sequence DB",
                NULL,
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:sequenceDB> <o:sequenceDB>",
                CITATION_MMSEQS2},
        {"swapresults",          swapresults,          &par.swapresult,          COMMAND_DB,
                "Reformat prefilter/alignment/cluster DB as if target DB had been searched through query DB",
                NULL,
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de>, Clovis Galiez & Eli Levy Karin",
                "<i:queryDB> <i:targetDB> <i:resultDB> <o:resultDB>",
                CITATION_MMSEQS2},
        {"swapdb",          swapdb,          &par.swapdb,          COMMAND_DB,
                "Create a DB where the key is from the first column of the input result DB",
                NULL,
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de>, Clovis Galiez & Eli Levy Karin",
                "<i:resultDB> <o:resultDB>",
                CITATION_MMSEQS2},
        {"mergedbs",             mergedbs,             &par.mergedbs,             COMMAND_DB,
                "Merge multiple DBs into a single DB, based on IDs (names) of entries",
                NULL,
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:sequenceDB> <o:resultDB> <i:resultDB1> ... <i:resultDBn>",
                CITATION_MMSEQS2},
        {"splitdb",              splitdb,              &par.splitdb,             COMMAND_DB,
                "Split a mmseqs DB into multiple DBs",
                NULL,
                "Milot Mirdita <milot@mirdita.de>",
                "<i:sequenceDB> <o:sequenceDB_1..N>",
                CITATION_MMSEQS2},
        {"subtractdbs",          subtractdbs,          &par.subtractdbs,          COMMAND_DB,
                "Generate a DB with entries of first DB not occurring in second DB",
                NULL,
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:resultDBLeft> <i:resultDBRight> <o:resultDB>",
                CITATION_MMSEQS2},
        {"filterdb",             filterdb,             &par.filterDb,             COMMAND_DB,
                "Filter a DB by conditioning (regex, numerical, ...) on one of its whitespace-separated columns",
                NULL,
                "Clovis Galiez & Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:resultDB> <o:resultDB>",
                CITATION_MMSEQS2},
        {"createsubdb",          createsubdb,          &par.onlyverbosity,        COMMAND_DB,
                "Create a subset of a DB from a file of IDs of entries",
                NULL,
                "Milot Mirdita <milot@mirdita.de>",
                "<i:subsetFile or DB> <i:resultDB> <o:resultDB>",
                CITATION_MMSEQS2},
        {"result2profile",       result2profile,       &par.result2profile,       COMMAND_DB,
                "Compute profile and consensus DB from a prefilter, alignment or cluster DB",
                NULL,
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:queryDB> <i:targetDB> <i:resultDB> <o:profileDB>",
                CITATION_MMSEQS2},
        {"result2pp",       result2pp,       &par.result2pp,       COMMAND_DB,
                "Merge the query profiles with target profiles according to search results and outputs an enriched profile DB",
                NULL,
                "Clovis Galiez & Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:queryDB> <i:targetDB> <i:resultDB> <o:profileDB>",
                CITATION_MMSEQS2},
        {"sequence2profile",       sequence2profile,       &par.sequence2profile,       COMMAND_DB,
                "Compute context state from sequence DB",
                NULL,
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:sequenceDB>  <o:profileDB>",
                CITATION_MMSEQS2},
        {"result2msa",           result2msa,           &par.result2msa,           COMMAND_DB,
                "Generate MSAs for queries by locally aligning their matched targets in prefilter/alignment/cluster DB",
                NULL,
                "Martin Steinegger (martin.steinegger@mpibpc.mpg.de) & Milot Mirdita <milot@mirdita.de> & Clovis Galiez",
                "<i:queryDB> <i:targetDB> <i:resultDB> <o:msaDB>",
                CITATION_MMSEQS2},
        {"convertmsa",           convertmsa,           &par.convertmsa,           COMMAND_DB,
                "Turns a MSA file into a MSA database.",
                "Builds an MSA database out of a MSA file in either Stockholm or PFAM format.",
                "Milot Mirdita <milot@mirdita.de>",
                "<i:msaFile[.gz]> <o:msaDB>",
                CITATION_MMSEQS2},
        {"msa2profile",          msa2profile,          &par.msa2profile,          COMMAND_DB,
                "Turns a MSA database into a MMseqs profile database.",
                "Builds a profile database from a database containing MSAs. The first sequence in the MSA is chosen as the query sequence. Gap columns (insertions) are discarded.",
                "Milot Mirdita <milot@mirdita.de>",
                "<i:msaDB> <o:profileDB>",
                CITATION_MMSEQS2},
        {"profile2pssm",         profile2pssm,         &par.profile2pssm,         COMMAND_DB,
                "Converts a profile database into a human readable tab-separated PSSM file.",
                NULL,
                "Milot Mirdita <milot@mirdita.de>",
                "<i:profileDB> <o:pssmFile>",
                CITATION_MMSEQS2},
        {"profile2cs",         profile2cs,         &par.profile2cs,         COMMAND_DB,
                "Converts a profile database into a column state sequence.",
                NULL,
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:profileDB> <o:csDB>",
                CITATION_MMSEQS2},
        {"result2stats",         result2stats,         &par.result2stats,         COMMAND_DB,
                "Compute statistics for each entry in a sequence, prefilter, alignment or cluster DB",
                NULL,
                "Clovis Galiez & Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:queryDB> <i:targetDB> <i:resultDB> <o:statsDB>",
                CITATION_MMSEQS2},
        {"offsetalignment",         offsetalignment,         &par.onlythreads,         COMMAND_HIDDEN,
                "Offset alignemnt by orf start position.",
                NULL,
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:queryDB> <i:targetDB> <i:alnDB> <o:alnDB>",
                CITATION_MMSEQS2},
        {"proteinaln2nucl",          proteinaln2nucl,          &par.onlythreads,            COMMAND_DB,
                "Map protein alignment to nucleotide alignment",
                NULL,
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de> ",
                "<i:queryDB> <i:targetDB> <i:alnDB> <o:alnDB>",
                CITATION_MMSEQS2},
        {"tsv2db",               tsv2db,               &par.tsv2db,               COMMAND_DB,
                "Turns a TSV file into a MMseqs database",
                NULL,
                "Milot Mirdita <milot@mirdita.de>",
                "<i:tsvFile> <o:resultDB>",
                CITATION_MMSEQS2
        },
        {"result2repseq",       result2repseq,      &par.onlythreads,          COMMAND_DB,
                "Get representative sequences for a result database",
                NULL,
                "Milot Mirdita <milot@mirdita.de> & Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:sequenceDB> <i:resultDB> <o:reprSeqDB>",
                CITATION_MMSEQS2},
// Special-purpose utilities
        {"rescorediagonal",           rescorediagonal,           &par.rescorediagonal,        COMMAND_SPECIAL,
                "Compute sequence identity for diagonal",
                NULL,
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:queryDB> <i:targetDB> <i:prefilterDB> <o:resultDB>",
                CITATION_MMSEQS2},
        {"alignbykmer",           alignbykmer,           &par.alignbykmer,        COMMAND_SPECIAL,
                "Predict sequence identity, score, alignment start and end by kmer alignment",
                NULL,
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:queryDB> <i:targetDB> <i:resultDB> <o:resultDB>",
                CITATION_MMSEQS2},
        {"diffseqdbs",           diffseqdbs,           &par.diff,        COMMAND_SPECIAL,
                "Find IDs of sequences kept, added and removed between two versions of sequence DB",
                "It creates 3 filtering files, that can be used in cunjunction with \"createsubdb\" tool.\nThe first file contains the keys that has been removed from DBold to DBnew.\nThe second file maps the keys of the kept sequences from DBold to DBnew.\nThe third file contains the keys of the sequences that have been added in DBnew.",
                "Clovis Galiez & Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:oldSequenceDB> <i:newSequenceDB> <o:rmSeqKeysFile> <o:keptSeqKeysFile> <o:newSeqKeysFile>",
                CITATION_MMSEQS2},
        {"concatdbs",            concatdbs,            &par.concatdbs,        COMMAND_SPECIAL,
                "Concatenate two DBs, giving new IDs to entries from second input DB",
                NULL,
                "Clovis Galiez & Martin Steinegger (martin.steinegger@mpibpc.mpg.de)",
                "<i:resultDB> <i:resultDB> <o:resultDB>",
                CITATION_MMSEQS2},
        {"sortresult",           sortresult,           &par.sortresult,           COMMAND_SPECIAL,
                "Sort a result database in the same order as prefilter or align would.",
                NULL,
                "Milot Mirdita <milot@mirdita.de>",
                "<i:resultbDB> <o:resultDB>",
                CITATION_MMSEQS2},
        {"summarizeresult",      summarizeresult,      &par.summarizeresult,      COMMAND_SPECIAL,
                "Extract annotations from result db",
                NULL,
                "Milot Mirdita <milot@mirdita.de> & Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:resultbDB> <o:resultDB>",
                CITATION_MMSEQS2|CITATION_UNICLUST},
        {"summarizetabs",        summarizetabs,        &par.summarizetabs,        COMMAND_SPECIAL,
                "Extract annotations from HHblits BAST-tab-formatted results",
                NULL,
                "Milot Mirdita <milot@mirdita.de> & Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:blastTabDB> <i:lengthFile> <o:summarizedBlastTabDB>",
                CITATION_MMSEQS2|CITATION_UNICLUST},
        {"gff2db",               gff2db,               &par.gff2ffindex,          COMMAND_SPECIAL,
                "Turn a gff3 (generic feature format) file into a gff3 DB",
                NULL,
                "Milot Mirdita <milot@mirdita.de>",
                "<i:gff3File> <i:sequenceDB> <o:sequenceDB>",
                CITATION_MMSEQS2},
        {"maskbygff",            maskbygff,            &par.gff2ffindex,          COMMAND_SPECIAL,
                "X out sequence regions in a sequence DB by features in a gff3 file",
                NULL,
                "Milot Mirdita <milot@mirdita.de>",
                "<i:gff3File> <i:sequenceDB> <o:sequenceDB>",
                CITATION_MMSEQS2},
        {"prefixid",             prefixid,             &par.prefixid,             COMMAND_SPECIAL,
                "For each entry in a DB prepend the entry ID to the entry itself",
                NULL,
                "Milot Mirdita <milot@mirdita.de>",
                "<i:resultDB> <o:resultDB>",
                CITATION_MMSEQS2},
        {"suffixid",             suffixid,             &par.prefixid,             COMMAND_SPECIAL,
                "For each entry in a DB append the entry ID to the entry itself",
                NULL,
                "Milot Mirdita <milot@mirdita.de>",
                "<i:resultDB> <o:resultDB>",
                CITATION_MMSEQS2},
        {"convertkb",            convertkb,            &par.convertkb,            COMMAND_SPECIAL,
                "Convert UniProt knowledge base files into MMseqs2 database format for the selected column types",
                NULL,
                "Milot Mirdita <milot@mirdita.de>",
                "<i:uniprotkb.dat[.gz]> ... <i:uniprotkb.dat[.gz]> <o:uniprotkbDB>",
                CITATION_MMSEQS2},
        {"summarizeheaders",     summarizeheaders,     &par.summarizeheaders,     COMMAND_SPECIAL,
                "Return a new summarized header DB from the UniProt headers of a cluster DB",
                NULL,
                "Milot Mirdita <milot@mirdita.de>",
                "<i:queryDB> <i:targetDB> <i:clusterDB> <o:headerDB>",
                CITATION_MMSEQS2|CITATION_UNICLUST},
        {"extractalignedregion", extractalignedregion, &par.extractalignedregion, COMMAND_SPECIAL,
                "Extract aligned sequence region",
                NULL,
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:queryDB> <i:targetDB> <i:resultDB> <o:domainDB>",
                CITATION_MMSEQS2},
        {"extractdomains",       extractdomains,       &par.extractdomains,       COMMAND_SPECIAL,
                "Extract highest scoring alignment region for each sequence from BLAST-tab file",
                NULL,
                "Milot Mirdita <milot@mirdita.de> & Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:domainDB> <i:msaDB> <o:domainDB>",
                CITATION_MMSEQS2|CITATION_UNICLUST},
        {"convertca3m",          convertca3m,          &par.onlythreads,          COMMAND_SPECIAL,
                "Converts a cA3M database into a MMseqs2 result database.",
                NULL,
                "Milot Mirdita <milot@mirdita.de>",
                "<i:ca3mDB> <o:resultDB>",
                CITATION_MMSEQS2},
        {"expandaln",           expandaln,             &par.expandaln,            COMMAND_SPECIAL,
                "Expands an alignment result based on another.",
                NULL,
                "Milot Mirdita <milot@mirdita.de>",
                "<i:queryDB> <i:targetDB> <i:resultDB> <i:resultDB|ca3mDB> <o:resultDB>",
                CITATION_MMSEQS2},
        {"dbtype",              dbtype,                &par.empty,                COMMAND_HIDDEN,
                "",
                NULL,
                "",
                "",
                CITATION_MMSEQS2},
        {"version",              versionstring,        &par.empty,                COMMAND_HIDDEN,
                "",
                NULL,
                "",
                "",
                CITATION_MMSEQS2},
        {"diskspaceavail",              diskspaceavail,        &par.empty,                COMMAND_HIDDEN,
                "",
                NULL,
                "",
                "",
                CITATION_MMSEQS2},
        {"shellcompletion",      shellcompletion,      &par.empty,                COMMAND_HIDDEN,
                "",
                NULL,
                "",
                "",
                CITATION_MMSEQS2}
};
