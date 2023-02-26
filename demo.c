#include <PCSC/winscard.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  SCARDCONTEXT s_card_ctx = 0;
  LPDWORD reader_names_len = (LPDWORD)SCARD_AUTOALLOCATE;
  LPSTR reader_names = NULL;
  int reture_value = EXIT_SUCCESS;
  long result =
      SCardEstablishContext(SCARD_SCOPE_SYSTEM, NULL, NULL, &s_card_ctx);

  if (result != SCARD_S_SUCCESS) {
    printf("SCardEstablishContext Fail : %08lX\n", result);
    return EXIT_FAILURE;
  }

  // Find out reader qty.
  result = SCardListReaders(s_card_ctx, NULL, (LPSTR)&reader_names,
                            (LPDWORD)&reader_names_len);
  if (result != SCARD_S_SUCCESS) {
    printf("SCardListReaders Fail : %08lX\n", result);
    return EXIT_FAILURE;
  } else {
    LPSTR reader_name = reader_names;
    LPSCARDHANDLE card;
    LPDWORD active_protocol;
    while ('\0' != *reader_name) {
      printf("Reader: %s\n", reader_name);
      result = SCardConnect(s_card_ctx, reader_name, SCARD_SHARE_EXCLUSIVE,
                            SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1,
                            (LPSCARDHANDLE)&card, (LPDWORD)&active_protocol);
      if (result != SCARD_S_SUCCESS) {
        printf("SCardConnect Fail : %08lX\n", result);
        reture_value = EXIT_FAILURE;
        goto FREE;
      }
      SCARD_IO_REQUEST s_card_io_request = {
          .dwProtocol = (unsigned long)active_protocol,
          .cbPciLength = sizeof(SCARD_IO_REQUEST)};
      BYTE test_command[] = {0x00, 0xB0, 0x00, 0x00, 0x10}; // read IC card serial number
      DWORD length = 4096;
      LPBYTE buffer = (LPBYTE)calloc((size_t)length, sizeof(LPBYTE));
      result = SCardTransmit((SCARDHANDLE)card, &s_card_io_request,
                             test_command, sizeof(test_command),
                             &s_card_io_request, buffer, (LPDWORD)&length);
      if (result == SCARD_S_SUCCESS) {
        printf("response: ");
        for (int i = 0; i < (int)length; i++) {
          printf("%02X ", buffer[i]);
        }
        printf("\n");
        if (buffer[(long)length - 2] == 0x90 &&
            buffer[(long)length - 1] == 0x00) {
          printf("smart card reader process command success\n");
        }

        result = SCardDisconnect((SCARDHANDLE)card, SCARD_UNPOWER_CARD);
        if (result != SCARD_S_SUCCESS) {
          printf("SCardDisconnect Fail : %08lX\n", result);
          reture_value = EXIT_FAILURE;
        }
        free(buffer);
        reader_name =
            reader_name + strnlen(reader_name, (size_t)reader_names_len) + 1;
      }
    }
  }

FREE:
  SCardFreeMemory(s_card_ctx, reader_names);
  SCardReleaseContext(s_card_ctx);
  return reture_value;
}
