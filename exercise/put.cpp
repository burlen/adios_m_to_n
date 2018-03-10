#include <adios.h>
#include <mpi.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>

#include "adios_tt.h"

using std::cerr;
using std::endl;

// MPI info
MPI_Comm g_comm = MPI_COMM_WORLD;
int g_rank = 0;
int g_n_ranks = 1;

#define ERROR(msg)                                  \
{std::cerr << "ERROR! [" << g_rank << "]["          \
    << __FILE__ << ":" << __LINE__ <<  "]" << endl  \
    << "" msg << endl;}


// --------------------------------------------------------------------------
template <typename n_t>
int define_array_adios(int64_t gh,
    int mesh_id, int array_id, unsigned int n_elem, uint64_t &buff_size)
{
    // tell ADIOS how we define the data
    // TODO -- dataset_<id>/array_<id>/number_of_elements
    // TODO -- dataset_<id>/array_<id>/data
    // TODO -- add the number of bytes needed

    return 0;
}

// --------------------------------------------------------------------------
int define_group_adios(const char *name,
    const char *method, int n_datasets_per, unsigned int n_elem,
    uint64_t &buff_size)
{
    buff_size = 0;

    // initialize adios
    adios_init_noxml(g_comm);

    adios_set_max_buffer_size(500);

    int64_t gh = 0;
    if (adios_declare_group(&gh, name, "",
        static_cast<ADIOS_STATISTICS_FLAG>(adios_flag_yes)))
        return -1;

    if (adios_select_method(gh, method, "", ""))
        return -1;

    // TODO -- number_of_datasets_per_writer
    // TODO -- number_of_writers
    // TODO -- add number of bytes needed

    for (int i = 0; i < n_datasets_per; ++i)
    {
        int dataset_id = n_datasets_per*g_rank + i;
        if (define_array_adios<double>(gh, dataset_id, 0, n_elem, buff_size))
            return -1;
    }

    return 0;
}

// --------------------------------------------------------------------------
template <typename n_t>
n_t *initialize_array(unsigned int n_elem)
{
    // allocate and initialize the array
    n_t *data = static_cast<n_t*>(malloc(n_elem*sizeof(n_t)));
    for (int i = 0; i < n_elem; ++i)
        data[i] = n_t(g_rank*n_elem + i);
    return data;
}

// --------------------------------------------------------------------------
template <typename n_t>
int write_array_adios(uint64_t fh, int dataset_id,
    int array_id, unsigned int n_elem, n_t *data)
{
    // TODO -- dataset_<id>/array_<id>/number_of_elements
    // TODO -- dataset_<id>/array_<id>/data
    return 0;
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    // initialize MPI stuff
    g_comm = MPI_COMM_WORLD;
    MPI_Comm_rank(g_comm, &g_rank);
    MPI_Comm_size(g_comm, &g_n_ranks);

    // process the comand line
    if (argc < 5)
    {
        cerr << "ERROR: put [file] [method] [array len] [n steps]" << endl;
        return -1;
    }
    const char *file = argv[1];
    const char *method = argv[2];
    int n_elem = atoi(argv[3]);
    int n_datasets_per = atoi(argv[4]);
    int n_steps = atoi(argv[5]);

    // describe the data layout to ADIOS, and compute per step buffer size
    uint64_t buff_size = 0;
    if (define_group_adios("data_group", method, n_datasets_per, n_elem, buff_size))
    {
        ERROR("Failed to define ADIOS group")
        return -1;
    }

    // write the time steps one by one
    for (int s = 0; s < n_steps; ++s)
    {
        // open file in append mode
        int64_t fh = 0;
        if (adios_open(&fh, "data_group", file, s == 0 ? "w" : "a", g_comm))
        {
            ERROR("Failed to open file " << file)
            return -1;
        }

        // set buffer size
        adios_group_size(fh, buff_size, &buff_size);

        // write the dataset metadata
        // TODO -- number_of_datasets_per_writer
        // TODO -- number_of_writers

        for (int i = 0; i < n_datasets_per; ++i)
        {
            double *data = initialize_array<double>(n_elem);

            int dataset_id = n_datasets_per*g_rank + i;
            if (write_array_adios<double>(fh, dataset_id, 0, n_elem, data))
            {
                ERROR("Failed to write array")
                return -1;
            }

            free(data);
        }

        // close the file
        adios_close(fh);

        cerr << g_rank << " put finished step " << s << endl;
    }

    adios_finalize(g_rank);

    MPI_Finalize();

    return 0;
}
