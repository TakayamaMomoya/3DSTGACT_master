xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 296;
 0.48044;18.75403;0.02356;,
 0.21579;18.75403;0.66247;,
 0.21579;-0.62610;0.66246;,
 0.48044;-0.62610;0.02355;,
 -0.42312;18.75403;0.92711;,
 -0.42312;-0.62610;0.92711;,
 -1.06204;18.75403;0.66247;,
 -1.06204;-0.62610;0.66246;,
 -1.32668;18.75403;0.02356;,
 -1.32668;-0.62610;0.02355;,
 -1.06204;18.75403;-0.61537;,
 -1.06204;-0.62610;-0.61537;,
 -0.42312;18.75403;-0.88001;,
 -0.42312;-0.62610;-0.88002;,
 0.21579;18.75403;-0.61537;,
 0.21579;-0.62610;-0.61537;,
 0.48044;18.75403;0.02356;,
 0.48044;-0.62610;0.02355;,
 -0.42312;18.75403;0.02356;,
 -0.42312;18.75403;0.02356;,
 -0.42312;18.75403;0.02356;,
 -0.42312;18.75403;0.02356;,
 -0.42312;18.75403;0.02356;,
 -0.42312;18.75403;0.02356;,
 -0.42312;18.75403;0.02356;,
 -0.42312;18.75403;0.02356;,
 -0.42312;-0.62610;0.02355;,
 -0.42312;-0.62610;0.02355;,
 -0.42312;-0.62610;0.02355;,
 -0.42312;-0.62610;0.02355;,
 -0.42312;-0.62610;0.02355;,
 -0.42312;-0.62610;0.02355;,
 -0.42312;-0.62610;0.02355;,
 -0.42312;-0.62610;0.02355;,
 6.02431;6.84888;0.02648;,
 3.44403;6.84887;3.89363;,
 3.44403;6.39944;3.89363;,
 6.02431;6.39944;0.02648;,
 -0.42312;6.84887;5.49546;,
 -0.42312;6.39944;5.49546;,
 -4.29028;6.84887;3.89363;,
 -4.29028;6.39944;3.89363;,
 -6.87056;6.84888;0.02647;,
 -6.87056;6.39944;0.02647;,
 -4.29028;6.84888;-3.84067;,
 -4.29028;6.39944;-3.84067;,
 -0.42312;6.84888;-5.44251;,
 -0.42312;6.39944;-5.44251;,
 3.44403;6.84888;-3.84067;,
 3.44403;6.39944;-3.84067;,
 6.02431;6.84888;0.02648;,
 6.02431;6.39944;0.02648;,
 -0.42312;6.84888;0.02648;,
 -0.42312;6.84888;0.02648;,
 -0.42312;6.84888;0.02648;,
 -0.42312;6.84888;0.02648;,
 -0.42312;6.84888;0.02648;,
 -0.42312;6.84888;0.02648;,
 -0.42312;6.84888;0.02648;,
 -0.42312;6.84888;0.02648;,
 -0.42312;6.39944;0.02648;,
 -0.42312;6.39944;0.02648;,
 -0.42312;6.39944;0.02648;,
 -0.42312;6.39944;0.02648;,
 -0.42312;6.39944;0.02648;,
 -0.42312;6.39944;0.02648;,
 -0.42312;6.39944;0.02648;,
 -0.42312;6.39944;0.02648;,
 2.91472;6.46433;0.02210;,
 1.93709;6.46433;2.38231;,
 0.78664;4.01461;1.23185;,
 1.28774;4.01462;0.02209;,
 -0.42312;6.46433;3.35995;,
 -0.42312;4.01461;1.73295;,
 -2.78334;6.46433;2.38231;,
 -1.63288;4.01461;1.23185;,
 -3.76097;6.46433;0.02210;,
 -2.13398;4.01462;0.02209;,
 -2.78333;6.46433;-2.33811;,
 -1.63288;4.01462;-1.18767;,
 -0.42312;6.46433;-3.31574;,
 -0.42312;4.01462;-1.68877;,
 1.93709;6.46433;-2.33811;,
 0.78664;4.01462;-1.18767;,
 2.91472;6.46433;0.02210;,
 1.28774;4.01462;0.02209;,
 -0.42312;6.46433;0.02210;,
 -0.42312;6.46433;0.02210;,
 -0.42312;6.46433;0.02210;,
 -0.42312;6.46433;0.02210;,
 -0.42312;6.46433;0.02210;,
 -0.42312;6.46433;0.02210;,
 -0.42312;6.46433;0.02210;,
 -0.42312;6.46433;0.02210;,
 -0.42312;4.01462;0.02209;,
 -0.42312;4.01462;0.02209;,
 -0.42312;4.01462;0.02209;,
 -0.42312;4.01462;0.02209;,
 -0.42312;4.01462;0.02209;,
 -0.42312;4.01462;0.02209;,
 -0.42312;4.01462;0.02209;,
 -0.42312;4.01462;0.02209;,
 4.91116;19.01010;0.02042;,
 3.34879;19.01010;3.79232;,
 3.34879;17.28230;3.79232;,
 4.91116;17.28230;0.02042;,
 -0.42312;19.01010;5.35469;,
 -0.42312;17.28230;5.35469;,
 -4.19503;19.01010;3.79232;,
 -4.19503;17.28230;3.79232;,
 -5.75741;19.01010;0.02042;,
 -5.75741;17.28230;0.02042;,
 -4.19504;19.01010;-3.75149;,
 -4.19504;17.28230;-3.75149;,
 -0.42312;19.01010;-5.31387;,
 -0.42312;17.28230;-5.31387;,
 3.34879;19.01010;-3.75149;,
 3.34879;17.28230;-3.75149;,
 -0.42312;19.01010;0.02042;,
 3.34879;19.01010;3.79232;,
 4.91116;19.01010;0.02042;,
 -0.42312;19.01010;5.35469;,
 -4.19503;19.01010;3.79232;,
 -5.75741;19.01010;0.02042;,
 -4.19504;19.01010;-3.75149;,
 -0.42312;19.01010;-5.31387;,
 3.34879;19.01010;-3.75149;,
 -0.42312;17.28230;0.02042;,
 4.91116;17.28230;0.02042;,
 3.34879;17.28230;3.79232;,
 -0.42312;17.28230;5.35469;,
 -4.19503;17.28230;3.79232;,
 -5.75741;17.28230;0.02042;,
 -4.19504;17.28230;-3.75149;,
 -0.42312;17.28230;-5.31387;,
 3.34879;17.28230;-3.75149;,
 2.61294;14.01080;1.42556;,
 1.40234;14.01080;3.73297;,
 1.40234;13.63145;3.73297;,
 2.61294;13.63147;0.67434;,
 1.40234;14.01080;3.73297;,
 -2.24859;14.01080;3.73297;,
 -2.24859;13.63145;3.73297;,
 1.40234;13.63145;3.73297;,
 -2.24859;14.01080;3.73297;,
 -3.45919;14.01080;1.42556;,
 -3.45919;13.63147;0.67434;,
 -2.24859;13.63145;3.73297;,
 -3.45919;14.01080;1.42556;,
 2.61294;14.01080;1.42556;,
 2.61294;13.63147;0.67434;,
 -3.45919;13.63147;0.67434;,
 1.40234;14.01080;3.73297;,
 2.61294;14.01080;1.42556;,
 2.61294;13.63147;0.67434;,
 1.40234;13.63145;3.73297;,
 1.23730;13.89373;3.25830;,
 1.23731;13.89373;57.11711;,
 1.23731;13.70782;57.11711;,
 1.23730;13.70783;3.25830;,
 1.23731;13.89373;57.11711;,
 -2.08353;13.89373;53.79393;,
 -2.08353;13.70782;53.79393;,
 1.23731;13.70782;57.11711;,
 -2.08353;13.89373;53.79393;,
 -2.08354;13.89373;3.25830;,
 -2.08354;13.70783;3.25830;,
 -2.08353;13.70782;53.79393;,
 -2.08354;13.89373;3.25830;,
 1.23730;13.89373;3.25830;,
 1.23730;13.70783;3.25830;,
 -2.08354;13.70783;3.25830;,
 1.23731;13.89373;57.11711;,
 1.23730;13.89373;3.25830;,
 1.23730;13.70783;3.25830;,
 1.23731;13.70782;57.11711;,
 -3.45919;14.01080;-1.37784;,
 -2.24858;14.01080;-3.68526;,
 -2.24858;13.63147;-3.68526;,
 -3.45919;13.63147;-0.62662;,
 -2.24858;14.01080;-3.68526;,
 1.40234;14.01080;-3.68526;,
 1.40234;13.63147;-3.68526;,
 -2.24858;13.63147;-3.68526;,
 1.40234;14.01080;-3.68526;,
 2.61295;14.01080;-1.37784;,
 2.61295;13.63147;-0.62662;,
 1.40234;13.63147;-3.68526;,
 2.61295;14.01080;-1.37784;,
 -3.45919;14.01080;-1.37784;,
 -3.45919;13.63147;-0.62662;,
 2.61295;13.63147;-0.62662;,
 -2.24858;14.01080;-3.68526;,
 -3.45919;14.01080;-1.37784;,
 -3.45919;13.63147;-0.62662;,
 -2.24858;13.63147;-3.68526;,
 -2.08354;13.89373;-3.21059;,
 -2.08356;13.89375;-57.06948;,
 -2.08356;13.70785;-57.06948;,
 -2.08354;13.70783;-3.21059;,
 -2.08356;13.89375;-57.06948;,
 1.23728;13.89375;-53.74628;,
 1.23728;13.70785;-53.74628;,
 -2.08356;13.70785;-57.06948;,
 1.23728;13.89375;-53.74628;,
 1.23730;13.89373;-3.21059;,
 1.23730;13.70783;-3.21059;,
 1.23728;13.70785;-53.74628;,
 1.23730;13.89373;-3.21059;,
 -2.08354;13.89373;-3.21059;,
 -2.08354;13.70783;-3.21059;,
 1.23730;13.70783;-3.21059;,
 -2.08356;13.89375;-57.06948;,
 -2.08354;13.89373;-3.21059;,
 -2.08354;13.70783;-3.21059;,
 -2.08356;13.70785;-57.06948;,
 -1.77693;14.01080;3.02748;,
 -4.08435;14.01080;1.81688;,
 -4.08435;13.63145;1.81688;,
 -1.02572;13.63145;3.02748;,
 -4.08435;14.01080;1.81688;,
 -4.08435;14.01080;-1.83405;,
 -4.08435;13.63147;-1.83405;,
 -4.08435;13.63145;1.81688;,
 -4.08435;14.01080;-1.83405;,
 -1.77693;14.01080;-3.04466;,
 -1.02572;13.63147;-3.04466;,
 -4.08435;13.63147;-1.83405;,
 -1.77693;14.01080;-3.04466;,
 -1.77693;14.01080;3.02748;,
 -1.02572;13.63145;3.02748;,
 -1.02572;13.63147;-3.04466;,
 -4.08435;14.01080;1.81688;,
 -1.77693;14.01080;3.02748;,
 -1.02572;13.63145;3.02748;,
 -4.08435;13.63145;1.81688;,
 -3.60968;13.89373;1.65183;,
 -57.46853;13.89373;1.65187;,
 -57.46853;13.70783;1.65187;,
 -3.60968;13.70783;1.65183;,
 -57.46853;13.89373;1.65187;,
 -54.14535;13.89373;-1.66897;,
 -54.14535;13.70783;-1.66897;,
 -57.46853;13.70783;1.65187;,
 -54.14535;13.89373;-1.66897;,
 -3.60968;13.89373;-1.66900;,
 -3.60968;13.70783;-1.66900;,
 -54.14535;13.70783;-1.66897;,
 -3.60968;13.89373;-1.66900;,
 -3.60968;13.89373;1.65183;,
 -3.60968;13.70783;1.65183;,
 -3.60968;13.70783;-1.66900;,
 -57.46853;13.89373;1.65187;,
 -3.60968;13.89373;1.65183;,
 -3.60968;13.70783;1.65183;,
 -57.46853;13.70783;1.65187;,
 0.96014;14.01080;-3.04466;,
 3.26756;14.01080;-1.83405;,
 3.26756;13.63147;-1.83405;,
 0.20893;13.63147;-3.04466;,
 3.26756;14.01080;-1.83405;,
 3.26756;14.01080;1.81687;,
 3.26756;13.63145;1.81687;,
 3.26756;13.63147;-1.83405;,
 3.26756;14.01080;1.81687;,
 0.96014;14.01080;3.02748;,
 0.20893;13.63145;3.02748;,
 3.26756;13.63145;1.81687;,
 0.96014;14.01080;3.02748;,
 0.96014;14.01080;-3.04466;,
 0.20893;13.63147;-3.04466;,
 0.20893;13.63145;3.02748;,
 3.26756;14.01080;-1.83405;,
 0.96014;14.01080;-3.04466;,
 0.20893;13.63147;-3.04466;,
 3.26756;13.63147;-1.83405;,
 2.79289;13.89373;-1.66901;,
 56.65171;13.89373;-1.66902;,
 56.65171;13.70783;-1.66902;,
 2.79289;13.70783;-1.66901;,
 56.65171;13.89373;-1.66902;,
 53.32853;13.89373;1.65181;,
 53.32853;13.70783;1.65181;,
 56.65171;13.70783;-1.66902;,
 53.32853;13.89373;1.65181;,
 2.79289;13.89373;1.65183;,
 2.79289;13.70783;1.65183;,
 53.32853;13.70783;1.65181;,
 2.79289;13.89373;1.65183;,
 2.79289;13.89373;-1.66901;,
 2.79289;13.70783;-1.66901;,
 2.79289;13.70783;1.65183;,
 56.65171;13.89373;-1.66902;,
 2.79289;13.89373;-1.66901;,
 2.79289;13.70783;-1.66901;,
 56.65171;13.70783;-1.66902;;
 
 144;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 3;18,1,0;,
 3;19,4,1;,
 3;20,6,4;,
 3;21,8,6;,
 3;22,10,8;,
 3;23,12,10;,
 3;24,14,12;,
 3;25,16,14;,
 3;26,3,2;,
 3;27,2,5;,
 3;28,5,7;,
 3;29,7,9;,
 3;30,9,11;,
 3;31,11,13;,
 3;32,13,15;,
 3;33,15,17;,
 4;34,35,36,37;,
 4;35,38,39,36;,
 4;38,40,41,39;,
 4;40,42,43,41;,
 4;42,44,45,43;,
 4;44,46,47,45;,
 4;46,48,49,47;,
 4;48,50,51,49;,
 3;52,35,34;,
 3;53,38,35;,
 3;54,40,38;,
 3;55,42,40;,
 3;56,44,42;,
 3;57,46,44;,
 3;58,48,46;,
 3;59,50,48;,
 3;60,37,36;,
 3;61,36,39;,
 3;62,39,41;,
 3;63,41,43;,
 3;64,43,45;,
 3;65,45,47;,
 3;66,47,49;,
 3;67,49,51;,
 4;68,69,70,71;,
 4;69,72,73,70;,
 4;72,74,75,73;,
 4;74,76,77,75;,
 4;76,78,79,77;,
 4;78,80,81,79;,
 4;80,82,83,81;,
 4;82,84,85,83;,
 3;86,69,68;,
 3;87,72,69;,
 3;88,74,72;,
 3;89,76,74;,
 3;90,78,76;,
 3;91,80,78;,
 3;92,82,80;,
 3;93,84,82;,
 3;94,71,70;,
 3;95,70,73;,
 3;96,73,75;,
 3;97,75,77;,
 3;98,77,79;,
 3;99,79,81;,
 3;100,81,83;,
 3;101,83,85;,
 4;102,103,104,105;,
 4;103,106,107,104;,
 4;106,108,109,107;,
 4;108,110,111,109;,
 4;110,112,113,111;,
 4;112,114,115,113;,
 4;114,116,117,115;,
 4;116,102,105,117;,
 3;118,119,120;,
 3;118,121,119;,
 3;118,122,121;,
 3;118,123,122;,
 3;118,124,123;,
 3;118,125,124;,
 3;118,126,125;,
 3;118,120,126;,
 3;127,128,129;,
 3;127,129,130;,
 3;127,130,131;,
 3;127,131,132;,
 3;127,132,133;,
 3;127,133,134;,
 3;127,134,135;,
 3;127,135,128;,
 4;136,137,138,139;,
 4;140,141,142,143;,
 4;144,145,146,147;,
 4;148,149,150,151;,
 4;148,141,152,153;,
 4;154,155,142,151;,
 4;156,157,158,159;,
 4;160,161,162,163;,
 4;164,165,166,167;,
 4;168,169,170,171;,
 4;168,161,172,173;,
 4;174,175,162,171;,
 4;176,177,178,179;,
 4;180,181,182,183;,
 4;184,185,186,187;,
 4;188,189,190,191;,
 4;188,181,192,193;,
 4;194,195,182,191;,
 4;196,197,198,199;,
 4;200,201,202,203;,
 4;204,205,206,207;,
 4;208,209,210,211;,
 4;208,201,212,213;,
 4;214,215,202,211;,
 4;216,217,218,219;,
 4;220,221,222,223;,
 4;224,225,226,227;,
 4;228,229,230,231;,
 4;228,221,232,233;,
 4;234,235,222,231;,
 4;236,237,238,239;,
 4;240,241,242,243;,
 4;244,245,246,247;,
 4;248,249,250,251;,
 4;248,241,252,253;,
 4;254,255,242,251;,
 4;256,257,258,259;,
 4;260,261,262,263;,
 4;264,265,266,267;,
 4;268,269,270,271;,
 4;268,261,272,273;,
 4;274,275,262,271;,
 4;276,277,278,279;,
 4;280,281,282,283;,
 4;284,285,286,287;,
 4;288,289,290,291;,
 4;288,281,292,293;,
 4;294,295,282,291;;
 
 MeshMaterialList {
  5;
  144;
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\MATERIAL\\Camouflage.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\MATERIAL\\Glass.png";
   }
  }
  Material {
   0.385600;0.385600;0.385600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\MATERIAL\\Camouflage.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\MATERIAL\\Camouflage.jpg";
   }
  }
 }
 MeshNormals {
  93;
  0.000000;1.000000;0.000000;,
  1.000000;-0.000000;0.000004;,
  0.707104;-0.000000;0.707109;,
  0.000003;-0.000000;1.000000;,
  -0.707107;-0.000000;0.707106;,
  -1.000000;-0.000000;0.000004;,
  -0.707110;0.000000;-0.707103;,
  0.000003;0.000000;-1.000000;,
  0.707107;0.000000;-0.707106;,
  0.000000;-1.000000;-0.000000;,
  -0.000000;1.000000;0.000001;,
  0.831834;0.000000;0.555025;,
  0.634647;0.000000;0.772802;,
  0.000000;0.000000;1.000000;,
  -0.634647;0.000000;0.772802;,
  -0.831834;0.000000;0.555024;,
  -0.634647;0.000000;-0.772802;,
  0.000000;0.000000;-1.000000;,
  0.634648;0.000000;-0.772801;,
  0.833016;-0.553249;-0.000001;,
  0.589032;-0.553250;0.589029;,
  0.000000;-0.553252;0.833014;,
  -0.589031;-0.553252;0.589029;,
  -0.833015;-0.553251;-0.000002;,
  -0.589031;-0.553248;-0.589032;,
  0.000000;-0.553247;-0.833017;,
  0.589032;-0.553248;-0.589032;,
  0.459287;-0.888288;-0.000003;,
  0.324765;-0.888290;0.324760;,
  0.000000;-0.888290;0.459282;,
  -0.324764;-0.888290;0.324759;,
  -0.459285;-0.888289;-0.000003;,
  -0.324764;-0.888288;-0.324765;,
  0.000000;-0.888288;-0.459287;,
  0.324765;-0.888288;-0.324765;,
  0.000000;-1.000000;-0.000003;,
  1.000000;0.000000;0.000000;,
  0.707106;0.000000;0.707107;,
  -0.000000;0.000000;1.000000;,
  -0.707106;0.000000;0.707108;,
  -1.000000;0.000000;0.000002;,
  -0.707107;0.000000;-0.707106;,
  0.000000;0.000000;-1.000000;,
  0.707108;0.000000;-0.707106;,
  0.861891;-0.310114;0.401213;,
  0.000000;0.000000;1.000000;,
  -0.861891;-0.310114;0.401213;,
  1.000000;0.000000;-0.000000;,
  -0.923947;0.000000;0.382521;,
  -1.000000;0.000000;0.000000;,
  -0.861889;-0.310118;-0.401216;,
  0.000000;0.000000;-1.000000;,
  0.861889;-0.310118;-0.401216;,
  -1.000000;0.000000;0.000000;,
  0.923947;0.000000;-0.382520;,
  1.000000;0.000000;-0.000000;,
  -0.401216;-0.310102;0.861895;,
  -1.000000;0.000000;0.000000;,
  -0.401217;-0.310115;-0.861889;,
  0.000001;0.000000;1.000000;,
  -0.382521;0.000000;-0.923947;,
  -0.000001;0.000000;-1.000000;,
  0.401217;-0.310115;-0.861889;,
  1.000000;0.000000;0.000000;,
  0.401218;-0.310104;0.861893;,
  -0.000000;0.000000;-1.000000;,
  0.382520;0.000000;0.923947;,
  0.000000;0.000000;1.000000;,
  -0.831833;0.000000;-0.555026;,
  0.831834;0.000000;-0.555025;,
  0.000000;1.000000;0.000002;,
  0.000000;1.000000;0.000001;,
  0.000000;1.000000;0.000002;,
  -0.000000;1.000000;0.000002;,
  -0.000000;1.000000;0.000001;,
  0.000000;0.972793;-0.231677;,
  0.000000;0.892652;-0.450747;,
  0.000000;-1.000000;-0.000006;,
  -0.707356;0.000000;0.706858;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.972793;0.231677;,
  0.000000;0.892652;0.450747;,
  0.707358;0.000000;-0.706856;,
  -0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.231684;0.972791;0.000001;,
  0.450761;0.892645;0.000001;,
  -0.000000;-1.000000;-0.000004;,
  -0.706858;0.000000;-0.707356;,
  -0.231684;0.972791;0.000001;,
  -0.450761;0.892645;0.000001;,
  -0.000000;-1.000000;-0.000004;,
  0.706856;0.000000;0.707357;;
  144;
  4;1,2,2,1;,
  4;2,3,3,2;,
  4;3,4,4,3;,
  4;4,5,5,4;,
  4;5,6,6,5;,
  4;6,7,7,6;,
  4;7,8,8,7;,
  4;8,1,1,8;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  4;11,12,12,11;,
  4;12,13,13,12;,
  4;13,14,14,13;,
  4;14,15,15,14;,
  4;68,16,16,68;,
  4;16,17,17,16;,
  4;17,18,18,17;,
  4;18,69,69,18;,
  3;10,70,71;,
  3;10,72,70;,
  3;10,73,72;,
  3;10,74,73;,
  3;10,0,74;,
  3;10,0,0;,
  3;10,0,0;,
  3;10,71,0;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  4;19,20,28,27;,
  4;20,21,29,28;,
  4;21,22,30,29;,
  4;22,23,31,30;,
  4;23,24,32,31;,
  4;24,25,33,32;,
  4;25,26,34,33;,
  4;26,19,27,34;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;35,27,28;,
  3;35,28,29;,
  3;35,29,30;,
  3;35,30,31;,
  3;35,31,32;,
  3;35,32,33;,
  3;35,33,34;,
  3;35,34,27;,
  4;36,37,37,36;,
  4;37,38,38,37;,
  4;38,39,39,38;,
  4;39,40,40,39;,
  4;40,41,41,40;,
  4;41,42,42,41;,
  4;42,43,43,42;,
  4;43,36,36,43;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  4;44,44,44,44;,
  4;45,45,45,45;,
  4;46,46,46,46;,
  4;75,75,76,76;,
  4;75,0,0,75;,
  4;77,77,77,77;,
  4;47,47,47,47;,
  4;78,48,48,78;,
  4;48,49,49,48;,
  4;51,51,51,51;,
  4;0,0,0,0;,
  4;79,79,79,79;,
  4;50,50,50,50;,
  4;51,51,51,51;,
  4;52,52,52,52;,
  4;80,80,81,81;,
  4;80,0,0,80;,
  4;9,9,9,9;,
  4;53,53,53,53;,
  4;82,54,54,82;,
  4;54,55,55,54;,
  4;45,45,45,45;,
  4;83,83,83,83;,
  4;84,84,84,84;,
  4;56,56,56,56;,
  4;57,57,57,57;,
  4;58,58,58,58;,
  4;85,85,86,86;,
  4;85,0,0,85;,
  4;87,87,87,87;,
  4;59,59,59,59;,
  4;88,60,60,88;,
  4;60,61,61,60;,
  4;63,63,63,63;,
  4;0,0,0,0;,
  4;9,9,9,9;,
  4;62,62,62,62;,
  4;63,63,63,63;,
  4;64,64,64,64;,
  4;89,89,90,90;,
  4;89,0,0,89;,
  4;91,91,91,91;,
  4;65,65,65,65;,
  4;92,66,66,92;,
  4;66,67,67,66;,
  4;57,57,57,57;,
  4;0,0,0,0;,
  4;9,9,9,9;;
 }
 MeshTextureCoords {
  296;
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.062500;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.062500;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.062500;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.501122;-0.450505;,
  0.311852;-0.450505;,
  0.311852;-0.364115;,
  0.501122;-0.364115;,
  0.232091;-0.450505;,
  0.232091;-0.364115;,
  0.308561;-0.450505;,
  0.308561;-0.364115;,
  0.496467;-0.450505;,
  0.496467;-0.364115;,
  0.685737;-0.450505;,
  0.685737;-0.364115;,
  0.765499;-0.450505;,
  0.765499;-0.364115;,
  0.689029;-0.450505;,
  0.689029;-0.364115;,
  0.478085;0.501902;,
  0.666377;0.313005;,
  0.744799;0.501473;,
  0.477657;0.235190;,
  0.289187;0.313610;,
  0.211371;0.502330;,
  0.289793;0.690799;,
  0.478514;0.768614;,
  0.666984;0.690193;,
  0.478154;0.501639;,
  0.744868;0.501211;,
  0.666446;0.312742;,
  0.477725;0.234927;,
  0.289256;0.313348;,
  0.211440;0.502067;,
  0.289862;0.690536;,
  0.478583;0.768352;,
  0.667052;0.689931;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;;
 }
}
