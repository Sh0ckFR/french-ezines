for a in a b c d e f g h i j k l m n o p q r s t u v w x y z A B C D E F G H I J K L M N O P
 Q R S T U V W X Y Z 1 2 3 4 5 6 7 8 9
do

	for b in a b c d e f g h i j k l m n o p q r s t u v w x y z A B C D E F G H I J K L M N O P Q R S T U V W X Y Z 1 2 3 4 5 6 7 8 9
	do

	
		for c in a b c d e f g h i j k l m n o p q r s t u v w x y z A B C D E F G H I J K L M N O P Q R S T U V W X Y Z 1 2 3 4 5 6 7 8 9
		do


			for d in a b c d e f g h i j k l m n o p q r s t u v w x y z A B C D E F G H I J K L M N O P Q R S T U V W X Y Z 1 2 3 4 5 6 7 8 9
			do
				echo -n "testing $a$b$c$d" >>/data/results
				echo $a$b$c$d | ./crk3.bf >>/data/results
			done


		done


	done

	echo "premiere lettre : $a" >>/data/cracked
	grep -A20 -B20 Crack3d /data/results >>/data/cracked
	>/data/results
	
done

