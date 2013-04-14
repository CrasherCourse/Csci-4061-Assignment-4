rm -rf testdir

# Create the first level of dir
mkdir testdir
cd testdir


  numdir=0
  while [ $numdir -lt 5 ]
  do
    # Create regular files 
    echo "Hello World" > message0
    echo "This is a fun assingment. I am almost done now." > message1
    echo "Goodbye."  > message2

    # Create directories
    dirname="dir-$numdir"
    echo "Creating directory $dirname"

    mkdir  $dirname
    cd  $dirname

    # Create regular files 
    echo "Hello World" > message0
    echo "This is a fun assingment. I am almost done now." > message1
    echo "Goodbye."  > message2

    # Create directories
         level1=0
         while [ $level1 -lt 4 ] 
         do
           dirname1="$dirname-$level1"
           echo "Creating directory $dirname1"
           mkdir $dirname1
           cd $dirname1

           # Create regular files 
           echo "Hello World" > message0
           echo "This is a fun assingment. I am almost done now." > message1
           echo "Goodbye."  > message2

           # Create directories
           level2=0
           while [ $level2 -lt 5 ]
           do
                dirname2="$dirname1-$level2"
                echo "Creating directory $dirname2"
                mkdir $dirname2
                cd $dirname2
                   # Create regular files 
                   echo "Hello World" > message0
                   echo "This is a fun assingment. I am almost done now." > message1
                   echo "Goodbye."  > message2
                cd ..
                level2=`expr $level2 + 1`
              done
          cd ..
          level1=`expr $level1 + 1`
         done
    cd ..
    numdir=`expr $numdir + 1`
  done
 
