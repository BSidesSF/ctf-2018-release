The flag is "DDrOREujZTOsgRkq3FjW" - no "FLAG:" prefix!

The first step is to determine which chain the password is in.  You do this by running the hash through the reduction function python script to get a new possible password.then checking the word at the end of the password chain (ie the only one stored in the rainbow table) to see if it matches...

Search the chains to find which chain stores the password for the hash 5663a63cd71f654627fdf0f90d9c2058

Here was our rainbow table:

Here is my rainbow table

    [['mark', 'Y2OuTCTYXeUMaN4DWr4B'],
     ['baggett', 'NpJZe4eesU8qJlNzGMPr'],
     ['ron', '8GzqN6E7voOKkdcPgOiP'],
     ['bowes', 'aLHplSmsM8ZrTZKxHsfr'],
     ['bsidessf', 'bDYidRnoiV3TS2lShBLC'],
     ['bsidesaugusta', '0AYiefH2gXVuvDrmGpUd'],
     ['gpyc', 'YVLXsgNdmXsdBiOzMOL5'],
     ['sec573', '5ZzlCF0gioDLULrSDacZ']]


The last password in each chain is ['Y2OuTCTYXeUMaN4DWr4B', 'NpJZe4eesU8qJlNzGMPr', '8GzqN6E7voOKkdcPgOiP', 'aLHplSmsM8ZrTZKxHsfr', 'bDYidRnoiV3TS2lShBLC', '0AYiefH2gXVuvDrmGpUd', 'YVLXsgNdmXsdBiOzMOL5', '5ZzlCF0gioDLULrSDacZ']

The hash 5663a63cd71f654627fdf0f90d9c2058 reduced to a password of SGEJEbTf3ds14s6fkN7D using this "reduction_function.py" program.   

python3 reduction_funciton.py 5663a63cd71f654627fdf0f90d9c2058
SGEJEbTf3ds14s6fkN7D

Comparing it to the last password in all chains (ie the only ones stored in the rainbow table).

Since that doesn't match any of those chains you MD5sum SGEJEbTf3ds14s6fkN7D and reduce  it again to another possible password.   Then compare it to the ends of all of the chains again.  You repeat this process until you find a matching password at the end of one of the chains.  

NOTE:Adding the -r argument to reduction function script will calculate the MD5 but you can use any md5 hash calculator.  It is just there as a convenience.

    python3 reduction_funciton.py -r 5663a63cd71f654627fdf0f90d9c2058
    c4e7df7e04c96b3ec3c7654dfda5b9cf

For this challenge they only have to repeat this 3 times to figure out which chain it is in. I can make this harder if you like.  I wanted the challenge to demonstrate understanding rainbow tables and not test patients but I can change it up if you want it to be more difficult.  In  SEC560 Ed covers all of this in some detail so 560 students who remember that material or have their books should be able to solve this.

Hash, reduce, Compare to end of chains again..

    SGEJEbTf3ds14s6fkN7D --md5--> c4e7df7e04c96b3ec3c7654dfda5b9cf --reduction--> oTXN4dwrFhau6qKwLAO0     No Match. 

Hash, reduce, Compare to end of chains again..

    oTXN4dwrFhau6qKwLAO0 --md5--> 6b1e6aa6cf26176b359685928e43a6e8 --reduction--> zavH4XBF0EQ5gumEc0g    No Match. 

Hash, reduce, Compare to end of chains again..

    zavH4XBF0EQ5gumEc0g --md5--> 1e5c24313207949677e349e99275344b --reduction--> aLHplSmsM8ZrTZKxHsfr   MATCH!!!
    ***Password aLHplSmsM8ZrTZKxHsfr matched the last item in chain that started with bowes 3 positions from the end of the chain!
    *** FOUND CHAIN! *** It is in the chain that starts with bowes

Now I know what password "bowes" started the chain that contains the password for my hash.   I just have to expand it by md5 hashing "bowes" and checking the result to see if it matches the hash im looking for.  If so, I cracked it. If not then I reduce (using this program) the hash of bowes to a new password , hash it and check again.  I repeat this process until I get the original password.

Inflating Chain for password bowes to find what hashes to 5663a63cd71f654627fdf0f90d9c2058

    bowes --md5--> 86e3e80d969d16641b892eac78333a17 --reduction--> CS4oHRAa3wVYLZoMQzzQ
    Password bowes Hashes to 86e3e80d969d16641b892eac78333a17.   Trying the password after bowes
    CS4oHRAa3wVYLZoMQzzQ --md5--> 63f42c47a3b7f96a5e2f9162b6697361 --reduction--> J5ZJM6CQ0sonA6IcBhNB
    Password CS4oHRAa3wVYLZoMQzzQ Hashes to 63f42c47a3b7f96a5e2f9162b6697361.   Trying the password after CS4oHRAa3wVYLZoMQzzQ
    J5ZJM6CQ0sonA6IcBhNB --md5--> db9490cdd2d46c7b433024ea8b6f5ad0 --reduction--> FqwIx6fabA1jI7nJEXhD
    Password J5ZJM6CQ0sonA6IcBhNB Hashes to db9490cdd2d46c7b433024ea8b6f5ad0.   Trying the password after J5ZJM6CQ0sonA6IcBhNB
    FqwIx6fabA1jI7nJEXhD --md5--> 5ada2890f0a4626b54747ff3aa2c0610 --reduction--> Y2gZeFPAlqegaZOQr4il
    Password FqwIx6fabA1jI7nJEXhD Hashes to 5ada2890f0a4626b54747ff3aa2c0610.   Trying the password after FqwIx6fabA1jI7nJEXhD
    Y2gZeFPAlqegaZOQr4il --md5--> 00f3d745a747908fc1d6927251eefa35 --reduction--> 4LAwxinDz5MVPP70esMS
    Password Y2gZeFPAlqegaZOQr4il Hashes to 00f3d745a747908fc1d6927251eefa35.   Trying the password after Y2gZeFPAlqegaZOQr4il
    4LAwxinDz5MVPP70esMS --md5--> dbe950039e3c341d6f64ac93387461f0 --reduction--> DDrOREujZTOsgRkq3FjW
    Password 4LAwxinDz5MVPP70esMS Hashes to dbe950039e3c341d6f64ac93387461f0.   Trying the password after 4LAwxinDz5MVPP70esMS
    DDrOREujZTOsgRkq3FjW --md5--> 5663a63cd71f654627fdf0f90d9c2058      MATCH !!!!!
    ******* PASSWORD FOUND : DDrOREujZTOsgRkq3FjW hashes to 5663a63cd71f654627fdf0f90d9c2058
    Password is DDrOREujZTOsgRkq3FjW

