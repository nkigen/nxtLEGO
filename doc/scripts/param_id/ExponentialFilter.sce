//Exponential Filter
    function [efdata] =ExponentialFilter(alph,avg)
        fdata = zeros(avg);
        fdata(1) = avg(1);
        for i=2:length(avg)
            fdata(i)=alph*avg(i)+(1-alph)*avg(i-1);
        end
        efdata=fdata;
    endfunction
