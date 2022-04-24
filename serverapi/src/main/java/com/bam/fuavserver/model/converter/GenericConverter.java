package com.bam.fuavserver.model.converter;

public interface GenericConverter<S, T> {

    T convert(S source);

}
