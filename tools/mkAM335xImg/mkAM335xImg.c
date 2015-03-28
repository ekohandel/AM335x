//
// This file creates a boot image for the TI AM335x processor. It can create
// an image for the boot process using a file system or the raw boot without
// a file system.
//
// example: ./mkAM335xImg -o boot.img -r -s 0x402F0400 boot.bin
//   - will create a raw boot image from boot.bin and add the appropriate
//     headers for the image to be loaded and executed at address 0x4020F0400
//
// example: ./mkAM335xImg -o MLO -s 0x402F0400 boot.bin
//   - will create a file system based boot image from boot.bin and add
//     the appropriate headers for the image to be loaded and executed at
//     address 0x4020F0400

#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/**************************** CHSETTINGS STRUCTURES *************************/
#define CH_SETTINGS              "CHSETTINGS"
#define CH_SIZE                  512
#define CH_SETTINGS_OFFSET_START 0x40
#define CH_SETTINGS_SIZE         0xE1
#define CH_SETTINGS_KEY          0xC0C0C0C1
#define CH_SETTINGS_VALID        0x1
#define CH_SETTINGS_VERSION      0x1

struct ch_toc_struct
{
  uint32_t start;
  uint32_t size;
  uint8_t  reserved[12];
  uint8_t  name[12];
  uint8_t  closing[32];
} __attribute__ ((__packed__));

struct ch_settings 
{
  uint32_t section_key;
  uint8_t  valid;
  uint8_t  version;
  uint8_t  reserved[0xD6];
} __attribute__ ((__packed__));

static void print_usage(const char* program_name, FILE* stream, int exit_code)
{
  fprintf(stream, "Usage: %s options [ input file ]\n", program_name);
  fprintf(stream,
          "Generate an image for raw or file system based boot of the\n"
          "TI AM335x Processor.\n\n");
  fprintf(stream,
          "  -h, --help             Display this usage information.\n"
          "  -o, --output filename  Write output to this file.\n"
          "  -r --raw               Make a raw boot image.\n"
          "  -s --sign location     Sign the image with a GP header.\n"
          "  -v --verbose           Print verbose messages.\n\n");
  fprintf(stream,
          "If no output file is indicated the input file name with a .img\n"
          "suffix is used.\n");

  exit(exit_code);
}

static void parse_options(int argc, char* argv[],
                         uint32_t* location,
                         const char** output_filename,
                         int* raw,
                         int* sign,
                         int* verbose)
{
  int next_option;

  const char* const short_options = "ho:rs:v";
  const struct option long_options[] =
  {
    {"help",     0, NULL, 'h'},
    {"output",   1, NULL, 'o'},
    {"raw",      0, NULL, 'r'},
    {"sign",     1, NULL, 's'},
    {"verbose",  1, NULL, 'v'},
    {NULL,       0, NULL, 0  }
  };

  do {
    next_option = getopt_long(argc, argv, short_options, long_options, NULL);
    switch (next_option)
    {
    case 'h': /* -h or --help */
      print_usage(argv[0], stdout, 0);
      break;

    case 'o': /* -o or --output */
      *output_filename = optarg;
      break;

    case 'r': /* -r or --raw */
      *raw = 1;
      break;

    case 's': /* -s or --sign */
      *sign = 1;
      sscanf(optarg, "%x", location);
      break;

    case 'v': /* -v or --verbose */
      *verbose = 1;
      break;

    case '?': /* Invalid option */
      print_usage(argv[0], stderr, 1);
      break;

    case -1: /* Done with options */
      break;

    default:
      abort();
      break;
    }
  } while (next_option != -1);
}

static void fillup_ch(uint8_t* ch_buffer)
{
  struct ch_toc_struct toc;
  struct ch_settings   ch;

  memset(ch_buffer, 0, CH_SIZE);

  // Form TOC section
  memset(&toc, 0, sizeof(toc));
  toc.start = CH_SETTINGS_OFFSET_START; 
  toc.size  = CH_SETTINGS_SIZE;
  strcpy((char*)toc.name, CH_SETTINGS);
  memset(&toc.closing, 0xFF, sizeof(toc.closing));

  // Form CH Settings
  memset(&ch, 0, sizeof(ch));
  ch.section_key = CH_SETTINGS_KEY;
  ch.valid       = CH_SETTINGS_VALID;
  ch.version     = CH_SETTINGS_VERSION;

  // Fill up the buffer
  memcpy(ch_buffer, &toc, sizeof(toc));
  memcpy(ch_buffer+sizeof(toc), &ch, sizeof(ch));
}

int main(int argc, char* argv[])
{
  const char* input_filename  = NULL;
  const char* output_filename = NULL;
        char* filename        = NULL;
  FILE*       ifd             = NULL;
  FILE*       ofd             = NULL;
  struct stat sinfo;
  uint32_t    location        = 0x0;
  int         raw             = 0;
  int         sign            = 0;
  int         verbose         = 0;
  uint32_t    len;
  uint32_t    word;
  uint8_t     byte;
  int         i;
  uint8_t     ch_buffer[CH_SIZE];

  parse_options(argc, argv,
                &location,
                &output_filename,
                &raw,
                &sign,
                &verbose);

  // Look for the input file
  if (optind < argc)
  {
    input_filename = argv[optind];
  }
  else
  {
    print_usage(argv[0], stderr, 1);
  }

  // If output file name was not set by options, form it from the
  // input file name.
  if (NULL == output_filename)
  {
    filename = malloc(strlen(input_filename) + 4);
    if (NULL != filename)
    {
      strcpy(filename, input_filename);
      strcat(filename, ".img");
    }
  }
  else
  {
    filename = malloc(strlen(output_filename));
    if (NULL != filename)

    {
      strcpy(filename, output_filename);
    }
  }

  // Something went wrong in filename generation
  if (NULL == filename)
  {
    fprintf(stderr, "Cannot allocate memory for file name");
    exit(1);
  }

  // Open output file
  ofd = fopen(filename, "wb");
  if (NULL == ofd)
  {
    fprintf(stderr, "Cannot open %s\n", filename);
    free(filename);
    exit(1);
  }

  stat(input_filename, &sinfo);
  len = sinfo.st_size;

  if (verbose > 0)
  {
    fprintf(stdout, "%s -> %s\n", input_filename, filename);
  }

  // Generate CH Section
  fillup_ch(ch_buffer);

  // Open input file
  ifd = fopen(input_filename, "rb");
  if (NULL == ifd)
  {
    fprintf(stderr, "Cannot open %s\n", input_filename);
    fclose(ofd);
    free(filename);
    exit(1);
  }

  // Place the CH buffer if this is a raw image
  if (raw > 0)
  {
    if (verbose > 0)
    {
      fprintf(stdout, "Making raw boot image\n");
    }
    fwrite(ch_buffer, 1, sizeof(ch_buffer), ofd);
  }

  // Add GP header if image is to be signed
  if (sign > 0)
  {
    if (verbose > 0)
    {
      fprintf(stdout, "Adding GP Header: Size = 0x%X, Location = 0x%X\n",
              len, location);
    }
    fwrite(&len, 4, 1, ofd);
    fwrite(&location, 4, 1, ofd);
  }

  // Copy image itself
  for (i = 0; i < len; i++)
  {
    fread(&byte, 1, 1, ifd);
    fwrite(&byte, 1, 1, ofd);
  }

  fclose(ifd);
  fclose(ofd);
  free(filename);
}
