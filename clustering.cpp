#include <bits/stdc++.h>
#include "shared.h"
#include "fcm.h"
#include "pcm.h"
#include "pfcm.h"
#include "descriptors.h"

using namespace std;

#include <argp.h>
#include <stdbool.h>

const char *argp_program_version = "Fuzzy Clustering";
const char *argp_program_bug_address = "<niltonvasques@dcc.ufba.br>";
static char doc[] = "Implementing some fuzzy clustering algorithms for flexible document organization.";
static char args_doc[] = "[FILENAME]...";
static struct argp_option options[] = { 
  { "fcm", 'f', 0, 0, "Run fuzzy c means."},
  { "pcm", 'p', 0, 0, "Run possibilistc c means."},
  { "pfcm", 'x', 0, 0, "Run possibilistc fuzzy c means."},
  { "input", 'i', "INPUT", 0, "Path for input data."},
  { "verbose", 'v', 0, 0, "More verbose execution."},
  { "a", 'a', "VALUE", 0, "PFCM a param. DEFAULT=1"},
  { "b", 'b', "VALUE", 0, "PFCM b param. DEFAULT=2"},
  { "m", 'm', "VALUE", 0, "Set m fuzziness param. DEFAULT=1.2"},
  { "M", 'M', "VALUE", 0, "Set min m fuzziness param. DEFAULT=m"},
  { "n", 'n', "VALUE", 0, "Set n fuzziness param. DEFAULT=1.2"},
  { "N", 'N', "VALUE", 0, "Set min n fuzziness param. DEFAULT=n"},
  { "clusters", 'c', "VALUE", 0, "Set max clusters. DEFAULT=3"},
  { "min-clusters", 'C', "VALUE", 0, "Set min clusters. DEFAULT=3"},
  { "cosine", 'k', 0, 0, "Set cosine norm. DEFAULT=euclidian"},
  { "euclidian", 'd', 0, 0, "Set euclidian norm. DEFAULT=euclidian"},
  { "jaccard", 'j', 0, 0, "Set jaccard norm. DEFAULT=euclidian"},
  { "random", 'r', "VALUE", 0, "Set random initials. DEFAULT=5"},
  { "output", 'o', "PATH", 0, "Set output path. DEFAULT=./"},
  { "epsilon", 'e', "VALUE", 0, "Set epsilon. DEFAULT=0.01"},
  { "partition", 'F', 0, 0, "First partition comes from fcm. DEFAULT=random"},
  { "sampling", 's', "INT", 0, "Sampling data. DEFAULT=num_docs"},
  { 0 } 
};


static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = (struct arguments *)state->input;
  switch (key) {
    case 'a': arguments->a = atof(arg); break;
    case 'b': arguments->b = atof(arg); break;
    case 'c': arguments->c = atof(arg); break;
    case 'C': arguments->min_clusters = atof(arg); break;
    case 'd': arguments->norm = EUCLIDIAN; break;
    case 'e': arguments->epsilon = atof(arg); break;
    case 'f': arguments->mode = FCM; break;
    case 'F': arguments->random = false; break;
    case 'i': arguments->input = arg; break;
    case 'j': arguments->norm = JACCARD; break;
    case 'k': arguments->norm = COSINE; break;
    case 'm': arguments->m = atof(arg); break;
    case 'M': arguments->min_m = atof(arg); break;
    case 'n': arguments->n = atof(arg); break;
    case 'N': arguments->min_n = atof(arg); break;
    case 'o': arguments->path = arg; break;
    case 'p': arguments->mode = PCM; break;
    case 'r': arguments->r = atof(arg); break;
    case 's': arguments->sampling = atof(arg); break;
    case 'v': arguments->verbose = true; break;
    case 'x': arguments->mode = PFCM; break;
    case ARGP_KEY_ARG: return 0;
    default: return ARGP_ERR_UNKNOWN;
  }   
  return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };

int main(int argc, char *argv[]){

  arguments.input = NULL;

  arguments.mode = FCM;
  arguments.verbose = false;
  arguments.a = 1;
  arguments.b = 2;
  arguments.m = 1.2;
  arguments.min_m = arguments.min_n = 0;
  arguments.n = 1.2;
  arguments.c = 3;
  arguments.min_clusters = 2;
  arguments.r = 5;
  arguments.norm = EUCLIDIAN;
  arguments.random = true;
  arguments.sampling = 0;
  arguments.epsilon = 0.01;
  string path = "";
  arguments.path = path.c_str();

  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  if(arguments.min_m == 0) arguments.min_m = arguments.m;
  if(arguments.min_n == 0) arguments.min_n = arguments.n;

  epsilon = arguments.epsilon;
  a = arguments.a;
  b = arguments.b;
  fuzziness = fuzziness_m = arguments.m;
  fuzziness_n = arguments.n;
  uint max_clusters = arguments.c;
  uint random_initials = arguments.r;

  //if(!arguments.input) {
  //  printf("--input FILE is required.\n");
  //  return 0;
  //}

  printf ("------------------------------------------------------------------------\n");
  if(arguments.mode == PCM){
    printf ("clustering data with pcm (possibilistic c means) method\n");
  }else if(arguments.mode == FCM){
    printf ("clustering data with fcm (fuzzy c means) method\n");
  }else if(arguments.mode == PFCM){
    printf ("clustering data with pfcm (possibilistic fuzzy c means) method\n");
  }
  string norm = "euclidian";
  string method = "fcm";
  if(arguments.norm == COSINE) norm = "cosine";
  if(arguments.norm == JACCARD) norm = "jaccard";
  if(arguments.mode == PCM) method = "pcm";
  if(arguments.mode == PFCM) method = "pfcm";
  printf ("parameters:\n");
  printf ("method\tm\tn\ta\tb\t\tnorm\n");
  printf ("%s\t%.2lf\t%.2lf\t%.2lf\t\t%.2lf\t%s\n", method.c_str(), fuzziness,
      fuzziness_n, a, b, norm.c_str());
  printf ("------------------------------------------------------------------------\n");

  printf("reading data\n");
  read_data();

  double max_fs = -2;
  double fs;
  uint min_clusters = arguments.min_clusters;
  max_groups = 2;
  printf("find optimal cluster number from %d to %d\n", min_clusters, max_clusters);
  double better_m = arguments.min_m;
  double better_n = arguments.min_n;
  FILE *f;
  string fname = "params.report";
  if ((f = fopen(fname.c_str(), "w")) == NULL) {
    fprintf(stderr, "Cannot create output file.\n");
    exit(1);
  }
  fprintf(f, "groups\tm\tn\taswc\n");
  for(fuzziness = arguments.min_m; fuzziness <= arguments.m; fuzziness += 0.1){
    fuzziness_m = fuzziness;
    for(fuzziness_n = arguments.min_n; fuzziness_n <= arguments.n; fuzziness_n += 0.1){
      for(uint i = min_clusters; i <= max_clusters; i++){
        printf("computing clustering with %d groups and %lf m %lf n\n", i, fuzziness, fuzziness_n);
        num_clusters = i;
        times(j, random_initials){
          if(arguments.mode == PCM){
            pcm();
          }else if(arguments.mode == FCM){
            fcm();
          }else if(arguments.mode == PFCM){
            pfcm();
          }
          fs = aswc();
          fprintf(f, "%d\t%lf\t%lf\t%lf\n", i, fuzziness, fuzziness_n, fs);
          if(arguments.verbose) cout << "FS: " << fs << endl;
          if(arguments.verbose) printf("%lf aswc %d groups\n", fs, i);
          if(fs > max_fs){
            max_fs = fs;
            max_groups = i;
            better_m = fuzziness;
            better_n = fuzziness_n;
            store_final_memberships();
            if(arguments.mode == PFCM){
              store_final_tipicalities();
            }
            save_matrix("", final_memberships, 3);
            //save_matrix("", final_tipicalities, num_docs);
            if(arguments.verbose) printf("max fs found: %lf aswc %lf m %lf n %d groups\n", fs, fuzziness, fuzziness_n, i);
          }
        }
      }
    }
  }
  fclose(f);
  printf ("------------------------------------------------------------------------\n");
  printf ("saving matrix data...\n");
  if(arguments.mode == PCM){
    ostringstream oss;
    oss << arguments.path << "tipicalities.matrix";
    save_matrix(oss.str(), final_memberships, num_docs);
  }else if(arguments.mode == FCM){
    ostringstream oss;
    oss << arguments.path << "memberships.matrix";
    save_matrix(oss.str(), final_memberships, num_docs);
  }else if(arguments.mode == PFCM){
    ostringstream oss, oss2, oss3;
    oss << arguments.path << "memberships.matrix";
    oss2 << arguments.path << "tipicalities.matrix";
    save_matrix(oss.str(), final_memberships, num_docs);
    save_matrix(oss2.str(), final_tipicalities, num_docs);
  }
  ostringstream oss;
  oss << arguments.path << "prototypes.matrix";
  save_matrix(oss.str(), prototypes, num_clusters);

  printf ("extracting descriptors...\n");
  if(arguments.mode == PCM){
    soft_fdcl();
    pdcl();
  }else if(arguments.mode == FCM){
    soft_fdcl();
  }else if(arguments.mode == PFCM){
    soft_fdcl();
    mixed_pdcl();
    ostringstream oss;
    oss << arguments.path << "merged.matrix";
    save_matrix(oss.str(), merged, num_docs);
  }

  printf ("the clustering process has finished\n");
  printf ("------------------------------------------------------------------------\n");
  printf ("max(aswc)\tgroups\tbetter(m)\tbetter(n)\n");
  printf ("%lf\t%d\t%lf\t%lf\n", max_fs, max_groups, better_m, better_n);
  printf ("------------------------------------------------------------------------\n");

  return 0;
}
