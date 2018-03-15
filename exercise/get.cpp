#include <adios.h>
#include <adios_read.h>
#include <mpi.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cstring>

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
struct ADIOSStream
{
  ADIOSStream() : File(nullptr), Method(static_cast<ADIOS_READ_METHOD>(-1)) {}

  ADIOSStream(ADIOS_FILE *file, ADIOS_READ_METHOD method)
    : File(file), Method(method) {}

  operator ADIOS_FILE*(){ return this->File; }
  operator const ADIOS_FILE*() const { return this->File; }
  operator ADIOS_READ_METHOD(){ return this->Method; }

  bool IsFile() { return this->Method == ADIOS_READ_METHOD_BP; }
  bool IsFlexpath() { return this->Method == ADIOS_READ_METHOD_FLEXPATH; }

  ADIOS_FILE *File;
  ADIOS_READ_METHOD Method;
};

// --------------------------------------------------------------------------
template <typename val_t>
int read_scalar_adios(ADIOS_FILE *fp, ADIOS_SELECTION *sel,
  const std::string &path, val_t &val)
{
  // TODO -- read a scalar
  return 0;
}

// --------------------------------------------------------------------------
template <typename val_t>
int read_scalar_adios(ADIOS_FILE *fp, const std::string &path, val_t &val)
{
  // TODO -- inquire a scalar
  return 0;
}

// --------------------------------------------------------------------------
ADIOS_READ_METHOD get_read_method(const char *method)
{
    if (strcmp(method, "BP") == 0)
        return ADIOS_READ_METHOD_BP;
    if (strcmp(method, "DATASPACES") == 0)
        return ADIOS_READ_METHOD_DATASPACES;
    if (strcmp(method, "FLEXPATH") == 0)
        return ADIOS_READ_METHOD_FLEXPATH;
    return static_cast<ADIOS_READ_METHOD>(-1);
}

// --------------------------------------------------------------------------
template <typename n_t>
void print_array(int dataset_id, int array_id, unsigned int n_elem, n_t *data)
{
    // print the array
    cerr << g_rank << " dataset_" << dataset_id << "/array_" << array_id
        << " " << n_elem << " " << adios_tt<n_t>::name() << endl;

    cerr << +data[0];
    for (int i = 1; i < n_elem; ++i)
        cerr  << (i % 32 == 0 ? "\n" : ", ") << +data[i];
    cerr << endl;
}

// --------------------------------------------------------------------------
template<typename n_t>
int read_array_adios(ADIOSStream *fp, int writer_id,
    int dataset_id, int array_id, unsigned int &n_elem, n_t *&data)
{
    // read array
    ADIOS_SELECTION *sel = nullptr;
    if (fp->IsFlexpath())
        sel = adios_selection_writeblock(writer_id);

    // TODO -- dataset_<id>/array_<id>/number_of_elements

    data = static_cast<n_t*>(malloc(n_elem*sizeof(n_t)));

    // TODO -- dataset_<id>/array_<id>/data

    if (sel)
        adios_selection_delete(sel);

    return 0;
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    // initialize MPI stuff
    g_comm = MPI_COMM_WORLD;
    MPI_Comm_rank(g_comm, &g_rank);
    MPI_Comm_size(g_comm, &g_n_ranks);

    if (argc < 3)
    {
        cerr << "ERROR: get [file] [method]" << endl;
        return -1;
    }

    const char *file_name = argv[1];
    const char *method_str = argv[2];
    int n_steps = 0;

    // initialize adios
    ADIOS_READ_METHOD method = get_read_method(method_str);
    adios_read_init_method(method, g_comm, "verbose=2");

    // open the file ADIOS_LOCKMODE_ALL
    ADIOS_FILE *fp = adios_read_open(file_name, method, g_comm,
      ADIOS_LOCKMODE_CURRENT, -1.0f);

    if (!fp)
    {
        ERROR("Failed to open " << file_name)
        return -1;
    }

    ADIOSStream *file = new ADIOSStream(fp, method);

    int s = 0;
    while (adios_errno == 0)
    {
        int n_datasets_per = 0;
        // TODO -- number_of_datasets_per_writer

        int n_writers = 0;
        // TODO -- number_of_writers

        // partiton the same number of datasets to each rank
        int n_datasets = n_writers*n_datasets_per;

        int n_per_rank = n_datasets/g_n_ranks;
        int n_left_over = n_datasets%g_n_ranks;

        int n_local = n_per_rank +
           (g_rank < n_left_over ? 1 : 0);

        int start_id = g_rank*n_per_rank +
          (g_rank < n_left_over ? g_rank : n_left_over);

        // read the local datasets
        if (n_local < 1)
        {
            cerr << g_rank << " has nothing to read" << endl;
        }
        else
        {
            for (int i = 0; i < n_local; ++i)
            {
                int dataset_id = start_id + i;
                int writer_id = dataset_id/n_datasets_per;

                unsigned int n_elem = 0;
                double *data = nullptr;
                if (read_array_adios(file, writer_id, dataset_id, 0, n_elem, data))
                    return -1;

                print_array(dataset_id, 0, n_elem, data);
            }
        }

        adios_release_step(fp);
        adios_advance_step(fp, 0, method == ADIOS_READ_METHOD_DATASPACES ? -1.0f : 0.0f);

        cerr << g_rank << " get finished step " << s << endl;
        s += 1;
    }

    adios_read_close(fp);
    adios_read_finalize_method(method);

    delete file;

    MPI_Finalize();

    return 0;
}
