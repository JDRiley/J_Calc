def power(i_val, i_pow):
    if(i_pow > 0):
        return i_val*power(i_val, i_pow - 1);
    else:
        return 1;

val = [];
def dopow(i_val, i_pow):
    i = 0;
    while(i < 10000):
        if not i%500:
            print(i);
        val.append(power(i_val, i_pow));
        i = i+ 1;
        
    return power(i_val, i_pow);

dopow(2,130);
print(val);
