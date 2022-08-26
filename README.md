# HyperionDecrypter

Hyperion is a emerging anti-tamper protection developed by Byfron technologies, one of their anti-tamper tricks is to encrypt the protected binary .text section.

This project will defeat this by triggering exceptions across the encrypted .text section -> Triggering these exceptions will result in protected regions to be decrypted and then re-encrypted by Hyperion.

![Decryption](https://i.imgur.com/STCdMVw.png)

## Usage

Inject the built project to your favourite Hyperion protected product. Once the decrypter has done its job the target process will be suspended before Hyperion can start the re-encrypting routine.
