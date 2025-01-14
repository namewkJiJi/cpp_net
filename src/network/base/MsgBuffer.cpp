#include "MsgBuffer.h"
#include <errno.h>
#include <sys/uio.h>

/*
来自muduo，根据自己的代码环境稍作修改
*/

using namespace tmms::network;

const char MsgBuffer::kCRLF[] = "\r\n";

const size_t MsgBuffer::kCheapPrepend;
const size_t MsgBuffer::kInitialSize;

ssize_t MsgBuffer::readFd(int fd, int* savedErrno)
{
  // saved an ioctl()/FIONREAD call to tell how much to read
  char extrabuf[65536];
  struct iovec vec[2];
  const size_t writable = writableBytes();
  vec[0].iov_base = begin()+writerIndex_;
  vec[0].iov_len = writable;
  vec[1].iov_base = extrabuf;
  vec[1].iov_len = sizeof extrabuf;
  // when there is enough space in this buffer, don't read into extrabuf.
  // when extrabuf is used, we read 128k-1 bytes at most.
  const int iovcnt = (writable < sizeof extrabuf) ? 2 : 1;
  const ssize_t n = readv(fd, vec, iovcnt);
  if (n < 0)
  {
    *savedErrno = errno;
  }
  else if ((const size_t)n <= writable)
  {
    writerIndex_ += n;
  }
  else
  {
    writerIndex_ = buffer_.size();
    append(extrabuf, n - writable);
  }
  // if (n == writable + sizeof extrabuf)
  // {
  //   goto line_30;
  // }
  return n;
}