function [w,t] = goalT_adaptive(w1, w2)
    % adaptive sample point density
    num_total = 58;
    band_ratio = 1.0*(w2 - w1)/280;
    allocate_sample_ratio = 1-power(band_ratio-1,4); %convex function
    bandin_num = ceil(num_total * allocate_sample_ratio);
    bandout_num = num_total - bandin_num;
    bandin_pace = 1.0*(w2 - w1)/bandin_num;
    bandout_pace = 1.0*(280 - w2 + w1)/bandout_num;
    begin_index = (w1 - 400)/bandout_pace;
    end_index = num_total - (680 - w2)/bandout_pace;
    
    t = []; % either 0 or 1 based on goal
    wv = []; % wavelength regarding with every sample point
    for i = 1:1:num_total
        if i < begin_index
            t = [t, 0];
            wv = [wv, 400 + i*bandout_pace];
        elseif i>=begin_index && i<=end_index
            t = [t, 1];
            wv = [wv, w1 + (i-begin_index)*bandin_pace];
        elseif i > end_index
            t = [t, 0];
            wv = [wv, 680 - (num_total-i)*bandout_pace];
        end
    end
    w = [wv,wv];
    t = [t,t];
end
