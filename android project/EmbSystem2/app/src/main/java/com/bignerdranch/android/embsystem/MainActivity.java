package com.bignerdranch.android.embsystem;

import android.content.ContentValues;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.util.Range;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;

public class MainActivity extends AppCompatActivity {

    private TextView tv_outPut;
    private int num;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button btn1 = (Button)findViewById(R.id.btn1);
        Button btn2 = (Button)findViewById(R.id.btn2);
        Button btn3 = (Button)findViewById(R.id.btn3);
        btn1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
                    // 위젯에 대한 참조.
                    tv_outPut = (TextView) findViewById(R.id.tv_outPut);
                    tv_outPut.setText("");

                    // URL 설정.
                    String url = "http://192.168.231.210:8080/embsystem2/embservlet2?type=info";
                    //String url = "http://10.0.2.2:8080/embsystem/embservlet?type=info";

                    num =1;
                    // AsyncTask를 통해 HttpURLConnection 수행.
                    NetworkTask networkTask = new NetworkTask(url, null);
                    networkTask.execute();

                } catch (Exception e) {

                }
            }
        });
        btn2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
                    // 위젯에 대한 참조.
                    tv_outPut = (TextView) findViewById(R.id.tv_outPut);
                    tv_outPut.setText("");

                    // URL 설정.
                    String url = "http://192.168.231.210:8080/embsystem2/embservlet2?type=info";
                    //String url = "http://10.0.2.2:8080/embsystem/embservlet?type=info";

                    num =2;
                    // AsyncTask를 통해 HttpURLConnection 수행.
                    NetworkTask networkTask = new NetworkTask(url, null);
                    networkTask.execute();

                } catch (Exception e) {

                }

            }
        });
        btn3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
                    // 위젯에 대한 참조.
                    tv_outPut = (TextView) findViewById(R.id.tv_outPut);
                    tv_outPut.setText("");

                    // URL 설정.
                    String url = "http://192.168.231.210:8080/embsystem2/embservlet2?type=info";
                    //String url = "http://10.0.2.2:8080/embsystem/embservlet?type=info";

                    num =3;
                    // AsyncTask를 통해 HttpURLConnection 수행.
                    NetworkTask networkTask = new NetworkTask(url, null);
                    networkTask.execute();

                } catch (Exception e) {

                }
            }
        });
    }

    private void jsonParsing(String json)
    {
        ArrayList<Ranking> ranking = new ArrayList<Ranking>();
        try{
            JSONObject jsonObject = new JSONObject(json);
            JSONArray rankArray = jsonObject.getJSONArray("Mdata");
            for(int i=0; i<rankArray.length(); i++)
            {
                JSONObject rankObject = rankArray.getJSONObject(i);

                Ranking rank = new Ranking();

                rank.setMode(rankObject.getString("mode"));
                rank.setName(rankObject.getString("name"));
                rank.setScore(rankObject.getString("score"));

                ranking.add(rank);
            }
        }catch (JSONException e) {
            e.printStackTrace();
        }

        for(Ranking r : ranking){
            if(num == 1){
                if(r.getMode().equals("1")){
                    String str = String.format(getResources().getString(R.string.textview_message),r.getName(),r.getScore());
                    tv_outPut.append(str);
                }
            }
            else if (num==2){
                if(r.getMode().equals("2")){
                    String str = String.format(getResources().getString(R.string.textview_message),r.getName(),r.getScore());
                    tv_outPut.append(str);
                }
            }
            else if (num==3){
                if(r.getMode().equals("3")){
                    String str = String.format(getResources().getString(R.string.textview_message),r.getName(),r.getScore());
                    tv_outPut.append(str);
                }
            }
        }
    }

    public class NetworkTask extends AsyncTask<Void, Void, String> {

        private String url;
        private ContentValues values;

        public NetworkTask(String url, ContentValues values) {
            this.url = url;
            this.values = values;
        }

        @Override
        protected String doInBackground(Void... params) {
            String result; // 요청 결과를 저장할 변수.
            RequestHttpURLConnection requestHttpURLConnection = new RequestHttpURLConnection();
            result = requestHttpURLConnection.request(url, values); // 해당 URL로 부터 결과물을 얻어온다.
            return result;
        }

        @Override
        protected void onPostExecute(String s) {
            super.onPostExecute(s);
            //doInBackground()로 부터 리턴된 값이 onPostExecute()의 매개변수로 넘어오므로 s를 출력한다.
            jsonParsing(s);
        }
    }
}