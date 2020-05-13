# ABCD
# echo "0: 4142 4344" | xxd -revert >nros2bytes.dat

# 0 to 10
echo "0000 0001 0002 0003 0004 0005 0006 0007 0008 0009 000A" \
  | xxd -revert -plain >nros2bytes.dat
