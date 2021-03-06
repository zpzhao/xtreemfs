/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.11
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package org.xtreemfs.common.libxtreemfs.jni.generated;

public class StringVector {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected StringVector(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(StringVector obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        xtreemfs_jniJNI.delete_StringVector(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public static StringVector from(java.util.Collection<String> in) {
    StringVector out = new StringVector();
    for (String entry : in) {
      out.add(entry);
    }
    return out;
  }

  public String[] toArray() {
    int size = (int) this.size();
    String[] out = new String[size];
    for (int i = 0; i < size; ++i) {
      out[i] = this.get(i);
    }
    return out;
  }

  public StringVector() {
    this(xtreemfs_jniJNI.new_StringVector__SWIG_0(), true);
  }

  public StringVector(long n) {
    this(xtreemfs_jniJNI.new_StringVector__SWIG_1(n), true);
  }

  public long size() {
    return xtreemfs_jniJNI.StringVector_size(swigCPtr, this);
  }

  public long capacity() {
    return xtreemfs_jniJNI.StringVector_capacity(swigCPtr, this);
  }

  public void reserve(long n) {
    xtreemfs_jniJNI.StringVector_reserve(swigCPtr, this, n);
  }

  public boolean isEmpty() {
    return xtreemfs_jniJNI.StringVector_isEmpty(swigCPtr, this);
  }

  public void clear() {
    xtreemfs_jniJNI.StringVector_clear(swigCPtr, this);
  }

  public void add(String x) {
    xtreemfs_jniJNI.StringVector_add(swigCPtr, this, x);
  }

  public String get(int i) {
    return xtreemfs_jniJNI.StringVector_get(swigCPtr, this, i);
  }

  public void set(int i, String val) {
    xtreemfs_jniJNI.StringVector_set(swigCPtr, this, i, val);
  }

}
