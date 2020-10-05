NR%3==0{
  b[NR / 3 + 1] = $1;
}
NR%3==1{
  r[NR / 3 + 1] = $1;
  # printf("r[%d] = \"%s\"\n", NR / 3 + 1, r[NR / 3 + 1]);
}
NR%3==2{
  g[NR / 3 + 1] = $1;
}

END {
  # for(i=1;i<=37;i++) {
  for(i in r) {
    if(i in r) {
      print "r contains i"
    } else {
      print "NOPE!"
    }
    printf("%d -> %s\n", i, r[i]);
  }

  printf("%s\n", r[27]);
}
