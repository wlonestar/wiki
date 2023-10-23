import sys

BATCH_SIZE = 100000

checked = 0
read, write = 0, 0
last_ch = '('

while True:
  for ch in sys.stdin.read(BATCH_SIZE):
    match ch:
      case '(': # 写操作
        write += 1
        if last_ch == '(': assert 0 # 上一个必须是读操作
      case ')': # 读操作
        read += 1
      case _: assert 0
    last_ch = ch
  checked += BATCH_SIZE
  print(f'read={read},write={write},checked={checked}.')  
