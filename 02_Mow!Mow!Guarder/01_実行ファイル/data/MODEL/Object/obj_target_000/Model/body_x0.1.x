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
 104;
 4.00000;1.00000;-3.00000;,
 4.00000;0.00000;-1.00000;,
 4.00000;0.00000;-4.00000;,
 4.00000;1.00000;-1.00000;,
 4.00000;1.00000;1.00000;,
 4.00000;0.00000;4.00000;,
 4.00000;0.00000;1.00000;,
 4.00000;1.00000;3.00000;,
 4.00000;3.00000;-4.00000;,
 4.00000;3.00000;-3.00000;,
 4.00000;3.00000;3.00000;,
 4.00000;3.00000;4.00000;,
 4.00000;7.00000;-3.00000;,
 4.00000;5.00000;-3.00000;,
 4.00000;5.00000;-4.00000;,
 4.00000;7.00000;3.00000;,
 4.00000;5.00000;4.00000;,
 4.00000;5.00000;3.00000;,
 4.00000;8.00000;-4.00000;,
 4.00000;7.00000;-1.00000;,
 4.00000;8.00000;-1.00000;,
 4.00000;8.00000;1.00000;,
 4.00000;7.00000;1.00000;,
 4.00000;8.00000;4.00000;,
 -4.00000;0.00000;-4.00000;,
 -4.00000;0.00000;-1.00000;,
 -4.00000;1.00000;-3.00000;,
 -4.00000;1.00000;-1.00000;,
 -4.00000;0.00000;1.00000;,
 -4.00000;0.00000;4.00000;,
 -4.00000;1.00000;1.00000;,
 -4.00000;1.00000;3.00000;,
 -4.00000;3.00000;-4.00000;,
 -4.00000;3.00000;-3.00000;,
 -4.00000;3.00000;3.00000;,
 -4.00000;3.00000;4.00000;,
 -4.00000;5.00000;-4.00000;,
 -4.00000;5.00000;-3.00000;,
 -4.00000;7.00000;-3.00000;,
 -4.00000;5.00000;3.00000;,
 -4.00000;5.00000;4.00000;,
 -4.00000;7.00000;3.00000;,
 -4.00000;7.00000;-1.00000;,
 -4.00000;8.00000;-4.00000;,
 -4.00000;8.00000;-1.00000;,
 -4.00000;7.00000;1.00000;,
 -4.00000;8.00000;1.00000;,
 -4.00000;8.00000;4.00000;,
 3.00000;1.00000;-4.00000;,
 3.00000;3.00000;-4.00000;,
 3.00000;5.00000;-4.00000;,
 3.00000;7.00000;-4.00000;,
 1.00000;0.00000;-4.00000;,
 1.00000;1.00000;-4.00000;,
 1.00000;7.00000;-4.00000;,
 1.00000;8.00000;-4.00000;,
 -3.00000;1.00000;-4.00000;,
 -1.00000;1.00000;-4.00000;,
 -1.00000;0.00000;-4.00000;,
 -3.00000;7.00000;-4.00000;,
 -1.00000;8.00000;-4.00000;,
 -1.00000;7.00000;-4.00000;,
 -3.00000;3.00000;-4.00000;,
 -3.00000;5.00000;-4.00000;,
 3.00000;1.00000;4.00000;,
 3.00000;3.00000;4.00000;,
 3.00000;5.00000;4.00000;,
 3.00000;7.00000;4.00000;,
 1.00000;0.00000;4.00000;,
 1.00000;1.00000;4.00000;,
 1.00000;7.00000;4.00000;,
 1.00000;8.00000;4.00000;,
 -1.00000;0.00000;4.00000;,
 -1.00000;1.00000;4.00000;,
 -3.00000;1.00000;4.00000;,
 -1.00000;7.00000;4.00000;,
 -1.00000;8.00000;4.00000;,
 -3.00000;7.00000;4.00000;,
 -3.00000;3.00000;4.00000;,
 -3.00000;5.00000;4.00000;,
 3.00000;0.00000;-3.00000;,
 1.00000;0.00000;-3.00000;,
 -1.00000;0.00000;-3.00000;,
 -3.00000;0.00000;-3.00000;,
 3.00000;0.00000;-1.00000;,
 -3.00000;0.00000;-1.00000;,
 3.00000;0.00000;3.00000;,
 3.00000;0.00000;1.00000;,
 -3.00000;0.00000;3.00000;,
 -3.00000;0.00000;1.00000;,
 1.00000;0.00000;3.00000;,
 -1.00000;0.00000;3.00000;,
 3.00000;8.00000;-3.00000;,
 1.00000;8.00000;-3.00000;,
 -1.00000;8.00000;-3.00000;,
 -3.00000;8.00000;-3.00000;,
 3.00000;8.00000;-1.00000;,
 -3.00000;8.00000;-1.00000;,
 3.00000;8.00000;1.00000;,
 3.00000;8.00000;3.00000;,
 -3.00000;8.00000;1.00000;,
 -3.00000;8.00000;3.00000;,
 1.00000;8.00000;3.00000;,
 -1.00000;8.00000;3.00000;;
 
 96;
 3;0,1,2;,
 3;3,1,0;,
 3;4,5,6;,
 3;7,5,4;,
 3;8,0,2;,
 3;9,0,8;,
 3;10,5,7;,
 3;11,5,10;,
 3;12,13,14;,
 3;15,16,17;,
 3;18,19,12;,
 3;18,12,14;,
 3;20,19,18;,
 3;21,15,22;,
 3;23,16,15;,
 3;23,15,21;,
 3;24,25,26;,
 3;26,25,27;,
 3;28,29,30;,
 3;30,29,31;,
 3;24,26,32;,
 3;32,26,33;,
 3;31,29,34;,
 3;34,29,35;,
 3;36,37,38;,
 3;39,40,41;,
 3;38,42,43;,
 3;36,38,43;,
 3;43,42,44;,
 3;45,41,46;,
 3;41,40,47;,
 3;46,41,47;,
 3;48,8,2;,
 3;49,8,48;,
 3;50,18,14;,
 3;51,18,50;,
 3;52,48,2;,
 3;53,48,52;,
 3;54,18,51;,
 3;55,18,54;,
 3;56,57,58;,
 3;59,60,61;,
 3;24,62,56;,
 3;24,56,58;,
 3;32,62,24;,
 3;36,59,63;,
 3;43,60,59;,
 3;43,59,36;,
 3;5,11,64;,
 3;64,11,65;,
 3;16,23,66;,
 3;66,23,67;,
 3;5,64,68;,
 3;68,64,69;,
 3;67,23,70;,
 3;70,23,71;,
 3;72,73,74;,
 3;75,76,77;,
 3;74,78,29;,
 3;72,74,29;,
 3;29,78,35;,
 3;79,77,40;,
 3;77,76,47;,
 3;40,77,47;,
 3;80,52,2;,
 3;81,52,80;,
 3;82,24,58;,
 3;83,24,82;,
 3;1,80,2;,
 3;84,80,1;,
 3;85,24,83;,
 3;25,24,85;,
 3;86,87,6;,
 3;88,28,89;,
 3;5,90,86;,
 3;5,86,6;,
 3;68,90,5;,
 3;72,88,91;,
 3;29,28,88;,
 3;29,88,72;,
 3;18,55,92;,
 3;92,55,93;,
 3;60,43,94;,
 3;94,43,95;,
 3;18,92,20;,
 3;20,92,96;,
 3;95,43,97;,
 3;97,43,44;,
 3;21,98,99;,
 3;100,46,101;,
 3;99,102,23;,
 3;21,99,23;,
 3;23,102,71;,
 3;103,101,76;,
 3;101,46,47;,
 3;76,101,47;;
 
 MeshMaterialList {
  1;
  96;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\Material\\mtl_pallet_000.png";
   }
  }
 }
 MeshNormals {
  10;
  1.000000;0.000000;-0.000000;,
  1.000000;0.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  -0.000000;-1.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;1.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;;
  96;
  3;0,0,0;,
  3;1,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,1;,
  3;0,0,0;,
  3;0,0,0;,
  3;8,8,8;,
  3;8,8,9;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;9,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;5,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,5,2;,
  3;2,2,5;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,6;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,6,4;,
  3;6,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;;
 }
 MeshTextureCoords {
  104;
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;;
 }
}
